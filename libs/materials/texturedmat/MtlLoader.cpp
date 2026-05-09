/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** MtlLoader
*/

#include "MtlLoader.hpp"

#include <fstream>
#include <sstream>
#include <string>

#include "exception/PluginException.hpp"
#include "math/Vector.hpp"

namespace raytracer::object::material {

    const std::unordered_map<
        std::string,
        std::function<void(MtlLoader &, std::istringstream &, MeshMaterial &)>>
        MtlLoader::_ops = {
            {"ns", &MtlLoader::handleNs},
            {"ka", &MtlLoader::handleKa},
            {"kd", &MtlLoader::handleKd},
            {"ks", &MtlLoader::handleKs},
            {"ke", &MtlLoader::handleKe},
            {"ni", &MtlLoader::handleNi},
            {"d", &MtlLoader::handleD},
            {"illum", &MtlLoader::handleIllum},
            {"map_Kd", &MtlLoader::handleMapKd},
            {"map_Bump", &MtlLoader::handleMapBump},
    };

    MtlLoader::MtlLoader(const std::string &filePath) {
        parseFile(filePath);
    }

    void MtlLoader::parseFile(const std::string &filePath) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            throw exception::PluginException("Failed to open MTL file: " +
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
            throw exception::PluginException("No material found in MTL file: " +
                                             filePath);
        }
    }

    void MtlLoader::processLine(const std::string &line,
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

    maths::Vector MtlLoader::parseVector(std::istringstream &iss) const {
        double r = 0.0;
        double g = 0.0;
        double b = 0.0;
        iss >> r >> g >> b;
        return maths::Vector(r, g, b);
    }

    void MtlLoader::finishCurrentMaterial(std::string &currentName,
                                          MeshMaterial &current) {
        _materials.emplace(currentName, current);
    }

    std::string MtlLoader::extractTexturePath(std::istringstream &iss) {
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

    void MtlLoader::handleNs(MtlLoader &, std::istringstream &iss,
                             MeshMaterial &current) {
        double value = 0.0;
        iss >> value;
        current.setNs(value);
    }

    void MtlLoader::handleKa(MtlLoader &loader, std::istringstream &iss,
                             MeshMaterial &current) {
        current.setKa(loader.parseVector(iss));
    }

    void MtlLoader::handleKd(MtlLoader &loader, std::istringstream &iss,
                             MeshMaterial &current) {
        current.setKd(loader.parseVector(iss));
    }

    void MtlLoader::handleKs(MtlLoader &loader, std::istringstream &iss,
                             MeshMaterial &current) {
        current.setKs(loader.parseVector(iss));
    }

    void MtlLoader::handleKe(MtlLoader &loader, std::istringstream &iss,
                             MeshMaterial &current) {
        current.setKe(loader.parseVector(iss));
    }

    void MtlLoader::handleNi(MtlLoader &, std::istringstream &iss,
                             MeshMaterial &current) {
        double value = 0.0;
        iss >> value;
        current.setNi(value);
    }

    void MtlLoader::handleD(MtlLoader &, std::istringstream &iss,
                            MeshMaterial &current) {
        double value = 0.0;
        iss >> value;
        current.setD(value);
    }

    void MtlLoader::handleIllum(MtlLoader &, std::istringstream &iss,
                                MeshMaterial &current) {
        double value = 0.0;
        iss >> value;
        current.setIllum(value);
    }

    void MtlLoader::handleMapKd(MtlLoader &, std::istringstream &iss,
                                MeshMaterial &current) {
        current.setMapKd(extractTexturePath(iss));
    }

    void MtlLoader::handleMapBump(MtlLoader &, std::istringstream &iss,
                                  MeshMaterial &current) {
        current.setMapBump(extractTexturePath(iss));
    }

    void MtlLoader::handleProperty(const std::string &keyword,
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

    const MeshMaterial &MtlLoader::get(const std::string &name) const {
        const auto it = _materials.find(name);
        if (it == _materials.end()) {
            throw exception::PluginException("Material not found: " + name);
        }
        return it->second;
    }

}  // namespace raytracer::object::material