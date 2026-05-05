/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** MltLoader
*/

#include "MltLoader.hpp"

#include <fstream>
#include <sstream>
#include <string>

#include "exception/PluginException.hpp"
#include "math/Vector.hpp"

namespace raytracer::object::material {

    const std::unordered_map<
        std::string,
        std::function<void(MltLoader &, std::istringstream &, MeshMaterial &)>>
        MltLoader::_ops = {
            {"Ns", &MltLoader::handleNs},
            {"Ka", &MltLoader::handleKa},
            {"Kd", &MltLoader::handleKd},
            {"Ks", &MltLoader::handleKs},
            {"Ke", &MltLoader::handleKe},
            {"Ni", &MltLoader::handleNi},
            {"d", &MltLoader::handleD},
            {"illum", &MltLoader::handleIllum},
            {"map_Kd", &MltLoader::handleMapKd},
            {"map_Bump", &MltLoader::handleMapBump},
    };

    MltLoader::MltLoader(const std::string &filePath) {
        parseFile(filePath);
    }

    void MltLoader::parseFile(const std::string &filePath) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            throw exception::PluginException("Failed to open MLT file: " +
                                             filePath);
        }

        MeshMaterial current;
        std::string currentName;
        std::string line;

        while (std::getline(file, line)) {
            processLine(line, filePath, currentName, current);
        }
        if (!currentName.empty()) {
            finishCurrentMaterial(currentName, current);
        }

        if (_materials.empty()) {
            throw exception::PluginException("No material found in MLT file: " +
                                             filePath);
        }
    }

    void MltLoader::processLine(const std::string &line,
                                const std::string &filePath,
                                std::string &currentName,
                                MeshMaterial &current) {
        if (line.empty() || line[0] == '#') {
            return;
        }

        std::istringstream iss(line);
        std::string keyword;
        iss >> keyword;

        if (keyword == "newmtl") {
            std::string name;
            iss >> name;
            if (!currentName.empty()) {
                finishCurrentMaterial(currentName, current);
                current = MeshMaterial();
            }
            current.setName(name);
            currentName = name;
            return;
        }

        if (currentName.empty()) {
            throw exception::PluginException(
                "MTL property before any 'newmtl' in file: " + filePath);
        }

        handleProperty(keyword, iss, filePath, current);
    }

    maths::Vector MltLoader::parseVector(std::istringstream &iss) const {
        double r = 0.0;
        double g = 0.0;
        double b = 0.0;
        iss >> r >> g >> b;
        return maths::Vector(r, g, b);
    }

    void MltLoader::finishCurrentMaterial(std::string &currentName,
                                          MeshMaterial &current) {
        _materials.emplace(currentName, current);
    }

    std::string MltLoader::extractTexturePath(std::istringstream &iss) {
        std::string rest;
        std::getline(iss, rest);

        const auto start = rest.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) {
            return {};
        }

        const auto end = rest.find_last_not_of(" \t\r\n");
        std::string trimmed = rest.substr(start, end - start + 1);
        const auto pathStart = trimmed.find_last_of(' ');
        if (pathStart == std::string::npos) {
            return trimmed;
        }
        return trimmed.substr(pathStart + 1);
    }

    void MltLoader::handleNs(MltLoader &, std::istringstream &iss,
                             MeshMaterial &current) {
        double value = 0.0;
        iss >> value;
        current.setNs(value);
    }

    void MltLoader::handleKa(MltLoader &loader, std::istringstream &iss,
                             MeshMaterial &current) {
        current.setKa(loader.parseVector(iss));
    }

    void MltLoader::handleKd(MltLoader &loader, std::istringstream &iss,
                             MeshMaterial &current) {
        current.setKd(loader.parseVector(iss));
    }

    void MltLoader::handleKs(MltLoader &loader, std::istringstream &iss,
                             MeshMaterial &current) {
        current.setKs(loader.parseVector(iss));
    }

    void MltLoader::handleKe(MltLoader &loader, std::istringstream &iss,
                             MeshMaterial &current) {
        current.setKe(loader.parseVector(iss));
    }

    void MltLoader::handleNi(MltLoader &, std::istringstream &iss,
                             MeshMaterial &current) {
        double value = 0.0;
        iss >> value;
        current.setNi(value);
    }

    void MltLoader::handleD(MltLoader &, std::istringstream &iss,
                            MeshMaterial &current) {
        double value = 0.0;
        iss >> value;
        current.setD(value);
    }

    void MltLoader::handleIllum(MltLoader &, std::istringstream &iss,
                                MeshMaterial &current) {
        double value = 0.0;
        iss >> value;
        current.setIllum(value);
    }

    void MltLoader::handleMapKd(MltLoader &, std::istringstream &iss,
                                MeshMaterial &current) {
        current.setMapKd(extractTexturePath(iss));
    }

    void MltLoader::handleMapBump(MltLoader &, std::istringstream &iss,
                                  MeshMaterial &current) {
        current.setMapBump(extractTexturePath(iss));
    }

    void MltLoader::handleProperty(const std::string &keyword,
                                   std::istringstream &iss,
                                   const std::string &filePath,
                                   MeshMaterial &current) {
        const auto it = _ops.find(keyword);
        if (it == _ops.end()) {
            throw exception::PluginException(
                "Unknown or unsupported keyword in MTL file: " + keyword +
                " in file: " + filePath);
        }

        it->second(*this, iss, current);
    }

    const MeshMaterial &MltLoader::get(const std::string &name) const {
        const auto it = _materials.find(name);
        if (it == _materials.end()) {
            throw exception::PluginException("Material not found: " + name);
        }
        return it->second;
    }

}  // namespace raytracer::object::material