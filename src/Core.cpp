/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Core
*/

#include "Core.hpp"

#include <unistd.h>

#include <algorithm>
#include <any>
#include <cstddef>
#include <exception>
#include <filesystem>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "exception/ParsingException.hpp"
#include "exporter/ExportPPM.hpp"
#include "exporter/IExport.hpp"
#include "object/IScene.hpp"
#include "parser/ConfigParser.hpp"
#include "plugin/ObjectFactory.hpp"
#include "plugin/PluginManager.hpp"
#include "visual/CliVisual.hpp"
#include "visual/IVisual.hpp"
#include "visual/SFMLVisual.hpp"

namespace raytracer {

    void Core::init(const std::vector<std::string> &argv,
                    const std::filesystem::path &pluginsPath) {
        this->cmdArgsHandling(argv);
        if (this->_export == nullptr)
            this->_export = std::make_unique<exporter::ExportPPM>();
        if (this->_visual == nullptr)
            this->_visual = std::make_unique<visual::CliVisual>();

        this->_renderer.setPrintProgressCallback(
            [this](int activeWorkers, Render &render) {
                return this->_visual->printProgress(activeWorkers, render);
            });
        this->_plugManager.updatePluginList(pluginsPath);
        this->_plugManager.fillFactory(this->_objFactory);

        this->_parser.setBuildCallback(
            [this](const std::string &name,
                   const std::map<std::string, std::any> &param)
                -> std::shared_ptr<object::IObject> {
                return this->_objFactory.build(name, param);
            });
        for (auto &scene : this->_scenes) {
            scene.scene = this->_parser.parse(scene.filePath);
            scene.lastUpdate = std::filesystem::last_write_time(scene.filePath);
            scene.scene->buildBVH(scene.scene->bvhStrategy());
        }
    }

    bool Core::updateSceneIfUpdated(SceneInstance &sceneInfo) {
        try {
            sceneInfo.scene = this->_parser.parse(sceneInfo.filePath);
            sceneInfo.scene->buildBVH(sceneInfo.scene->bvhStrategy());
            return true;
        } catch (const std::exception &e) {
            std::cerr << "Hot reload parse error: " << e.what() << "\n";
            return false;
        }
    }

    void Core::runPreview(SceneInstance &sceneInfo) {
        startFileUpdateWatcher(sceneInfo);

        while (!this->_visual->fullRender() && !this->_visual->stopLoop() &&
               !this->_visual->isBackRequested()) {
            if (this->_renderer.reloadRequested() &&
                !this->updateSceneIfUpdated(sceneInfo)) {
                this->_renderer.clearReload();
                break;
            }

            this->_renderer.clearReload();
            auto &scene = sceneInfo.scene;
            int cachedWidth = scene->cameras().at(0)->imageWidth();
            int cachedHeight = scene->cameras().at(0)->imageHeight();

            scene->cameras().at(0)->setImageHeight(cachedHeight / 2);
            scene->cameras().at(0)->setImageWidth(cachedWidth / 2);
            this->_renderer.render(*scene, PREVIEW_SAMPLE_PER_PIXEL);
            scene->cameras().at(0)->setImageHeight(cachedHeight);
            scene->cameras().at(0)->setImageWidth(cachedWidth);
        }

        stopFileUpdateWatcher();
    }

    void Core::runScene(SceneInstance &sceneInfo) {
        if (this->_visual->allowPreview()) {
            this->runPreview(sceneInfo);
        }
        auto &scene = sceneInfo.scene;

        if (!this->_visual->allowPreview() || this->_visual->fullRender())
            _renderer.render(*scene, scene->samplesPerPixel());
        if (!this->_renderer.renderedStopped() &&
            this->_visual->wantSave(this->_renderer))
            this->_export->writeFile(*scene, this->_renderer.pixels());
    }

    void Core::run() {
        if (this->_scenes.size() == 1) {
            this->runScene(*this->_scenes.begin());
            return;
        }
        while (!this->_visual->stopLoop()) {
            int sceneIndex =
                this->_visual->selectScene(this->_scenes, this->_renderer);

            if (sceneIndex < 0)
                return;
            if (static_cast<size_t>(sceneIndex) >= this->_scenes.size())
                break;

            this->runScene(this->_scenes.at(static_cast<size_t>(sceneIndex)));
        }
    }

    std::pair<bool, int> Core::helpMessage(
        const std::vector<std::string> &argv) {
        if (argv.empty()) {
            std::cerr << HELP_MESSAGE;
            return {true, 84};
        }
        if (std::any_of(argv.begin(), argv.end(),
                        [](const std::string &parameter) {
                            return parameter == "-h" || parameter == "--help";
                        })) {
            std::cout << HELP_MESSAGE;
            return {true, 0};
        }
        return {false, 0};
    }

    void Core::cmdArgsHandling(const std::vector<std::string> &argv) {
        for (size_t i = 0; i < argv.size(); ++i) {
            const std::string &param = argv[i];

            if (!param.starts_with("-")) {
                this->_scenes.emplace_back(SceneInstance{nullptr, {}, param});
                continue;
            }
            if (i + 1 >= argv.size())
                throw exception::ParsingException(HELP_MESSAGE);
            handleFlag(param, argv[++i]);
        }
        if (this->_scenes.empty())
            throw exception::ParsingException(HELP_MESSAGE);
    }

    void Core::handleFlag(const std::string &flag, const std::string &value) {
        static const std::map<
            std::string, std::function<std::unique_ptr<exporter::IExport>()>>
            exporters = {
                {"ppm", [] { return std::make_unique<exporter::ExportPPM>(); }},
            };
        static const std::map<std::string,
                              std::function<std::unique_ptr<visual::IVisual>()>>
            visuals = {
                {"cli", [] { return std::make_unique<visual::CliVisual>(); }},
                {"sfml", [] { return std::make_unique<visual::SFMLVisual>(); }},
            };

        auto pick = [&](auto &map, auto &target) {
            if (target != nullptr)
                throw exception::ParsingException(HELP_MESSAGE);
            auto it = map.find(value);
            if (it == map.end())
                throw exception::ParsingException(HELP_MESSAGE);
            target = it->second();
        };

        if (flag == EXPORT_FLAG)
            pick(exporters, _export);
        else if (flag == VISUAL_FLAG)
            pick(visuals, _visual);
        else
            throw exception::ParsingException(HELP_MESSAGE);
    }

    void Core::startFileUpdateWatcher(SceneInstance &scene) {
        this->_fileUpdateRunning.store(true);
        this->_fileUpdateWatcher = std::thread([this, &scene]() {
            while (this->_fileUpdateRunning.load()) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                try {
                    auto t = std::filesystem::last_write_time(scene.filePath);
                    if (t != scene.lastUpdate) {
                        scene.lastUpdate = t;
                        this->_renderer.requestReload();
                    }
                } catch (const std::exception &err) {
                    std::cout << "Error: Fail to detect if file been updated, "
                              << err.what() << "\n";
                    this->_renderer.stopRendering();
                }
            }
        });
    }

    void Core::stopFileUpdateWatcher() {
        this->_fileUpdateRunning.store(false);
        if (this->_fileUpdateWatcher.joinable())
            this->_fileUpdateWatcher.join();
    }

}  // namespace raytracer
