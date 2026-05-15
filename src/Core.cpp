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
#include "parser/ConfigParser.hpp"
#include "plugin/ObjectFactory.hpp"
#include "plugin/PluginManager.hpp"
#include "visual/CliVisual.hpp"
#include "visual/IVisual.hpp"
#include "visual/SFMLVisual.hpp"

namespace raytracer {

    void Core::init(const std::vector<std::string> &argv,
                    const std::filesystem::path &pluginsPath) {
        parsing::ConfigParser parser;

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

        parser.setBuildCallback(
            [this](const std::string &name,
                   const std::map<std::string, std::any> &param)
                -> std::shared_ptr<object::IObject> {
                return this->_objFactory.build(name, param);
            });
        this->_scenes = parser.parse(this->_givenFile);
        this->_scenes.at(0)->buildBVH(this->_scenes.at(0)->bvhStrategy());
    }

    void Core::runScene() {
        if (this->_visual->allowPreview()) {
            int cachedSample = this->_scenes.at(0)->samplesPerPixel();
            int cachedWidth =
                this->_scenes.at(0)->cameras().at(0)->imageWidth();
            int cachedHeight =
                this->_scenes.at(0)->cameras().at(0)->imageHeight();

            this->_scenes.at(0)->setSamplesPerPixel(PREVIEW_SAMPLE_PER_PIXEL);
            this->_scenes.at(0)->cameras().at(0)->setImageHeight(cachedHeight /
                                                                 2);
            this->_scenes.at(0)->cameras().at(0)->setImageWidth(cachedWidth /
                                                                2);

            this->_renderer.render(*(this->_scenes.at(0)),
                                   this->_scenes.at(0)->samplesPerPixel());

            this->_scenes.at(0)->setSamplesPerPixel(cachedSample);

            this->_scenes.at(0)->cameras().at(0)->setImageHeight(cachedHeight);
            this->_scenes.at(0)->cameras().at(0)->setImageWidth(cachedWidth);
        }
        if (!this->_visual->allowPreview() || this->_visual->fullRender()) {
            this->_renderer.render(*(this->_scenes.at(0)),
                                   this->_scenes.at(0)->samplesPerPixel());
        }
        if (!this->_renderer.renderedStopped() &&
            this->_visual->installFile(this->_renderer)) {
            this->_export->writeFile(*(this->_scenes.at(0)),
                                     this->_renderer.pixels());
            return;
        }
    }

    void Core::run() {
        if (this->_scenes.size() == 0) {
            this->runScene();
            return;
        }
        this->runScene();
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
        bool gotFile = false;

        for (size_t i = 0; i < argv.size(); ++i) {
            const std::string &param = argv[i];

            if (!param.starts_with("-")) {
                if (gotFile)
                    throw exception::ParsingException(HELP_MESSAGE);
                _givenFile = param;
                gotFile = true;
                continue;
            }
            if (i + 1 >= argv.size())
                throw exception::ParsingException(HELP_MESSAGE);
            handleFlag(param, argv[++i]);
        }
        if (!gotFile)
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

}  // namespace raytracer
