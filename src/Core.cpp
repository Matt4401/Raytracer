/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Core
*/

#include "Core.hpp"

#include <algorithm>
#include <any>
#include <cstddef>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

#include "exception/ParsingException.hpp"
#include "exporter/ExportPPM.hpp"
#include "exporter/IExport.hpp"
#include "parser/ConfigParser.hpp"
#include "plugin/ObjectFactory.hpp"
#include "plugin/PluginManager.hpp"

namespace raytracer {

    void Core::init(const std::vector<std::string> &argv,
                    const std::filesystem::path &pluginsPath) {
        parsing::ConfigParser parser;

        this->cmdArgsHandling(argv);
        if (this->_export == nullptr) {
            this->_export = std::make_unique<exporter::ExportPPM>();
        }

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

    void Core::run() {
        this->_renderer.render(*(this->_scenes.at(0)), 1, 50);
        this->_export->writeFile(*(this->_scenes.at(0)),
                                 this->_renderer.pixels());
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

}  // namespace raytracer
