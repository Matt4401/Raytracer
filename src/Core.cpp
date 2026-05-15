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
        if (!this->_renderer.renderedStopped()) {
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
        size_t nbArgs = argv.size();
        bool gotFile = false;

        using FlagHandler =
            std::function<void(size_t, const std::vector<std::string> &)>;
        std::map<std::string, FlagHandler> flagHandlers;

        flagHandlers.insert(
            {EXPORT_FLAG.data(),
             [this](size_t index, const std::vector<std::string> &argv) {
                 this->setExportViaFlag(index, argv);
             }});

        flagHandlers.insert(
            {VISUAL_FLAG.data(),
             [this](size_t index, const std::vector<std::string> &argv) {
                 this->setVisualViaFlag(index, argv);
             }});

        for (size_t index = 0; index < nbArgs; ++index) {
            const std::string &param = argv.at(index);

            if (!param.starts_with("-")) {
                if (gotFile)
                    throw exception::ParsingException(HELP_MESSAGE);
                this->_givenFile = param;
                gotFile = true;
                continue;
            }

            auto handler = flagHandlers.find(param);
            if (index + 1 >= nbArgs || handler == flagHandlers.end()) {
                throw exception::ParsingException(HELP_MESSAGE);
            } else {
                handler->second(index, argv);
                index++;
            }
        }
        if (!gotFile)
            throw exception::ParsingException(HELP_MESSAGE);
    }

    void Core::setExportViaFlag(size_t index,
                                const std::vector<std::string> &argv) {
        if (this->_export != nullptr)
            throw exception::ParsingException(HELP_MESSAGE);

        std::map<std::string, std::unique_ptr<exporter::IExport>> exportMap;
        exportMap.insert({"ppm", std::make_unique<exporter::ExportPPM>()});

        if (auto iter = exportMap.find(argv.at(index + 1));
            iter != exportMap.end()) {
            this->_export = std::move(iter->second);
        } else {
            throw exception::ParsingException(HELP_MESSAGE);
        }
    }

    void Core::setVisualViaFlag(size_t index,
                                const std::vector<std::string> &argv) {
        if (this->_visual != nullptr)
            throw exception::ParsingException(HELP_MESSAGE);

        std::map<std::string, std::unique_ptr<visual::IVisual>> exportMap;
        exportMap.insert({"cli", std::make_unique<visual::CliVisual>()});
        exportMap.insert({"sfml", std::make_unique<visual::SFMLVisual>()});

        if (auto iter = exportMap.find(argv.at(index + 1));
            iter != exportMap.end()) {
            this->_visual = std::move(iter->second);
        } else {
            throw exception::ParsingException(HELP_MESSAGE);
        }
    }

}  // namespace raytracer
