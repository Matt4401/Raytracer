/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** ObjLoader
*/

#include "ObjLoader.hpp"

#include <fstream>
#include <iostream>

namespace raytracer::object::primitive {

    const std::unordered_map<
        std::string, std::function<void(ObjLoader &, std::istringstream &)>>
        ObjLoader::_ops = {
            {"v", &ObjLoader::handleVertex},
            {"vn", &ObjLoader::handleNormal},
            {"vt", &ObjLoader::handleTexCoord},
            {"f", &ObjLoader::handleFace},
            {"usemtl", &ObjLoader::handleUseMtl},
    };

    ObjLoader::ObjLoader(const std::string &filePath) {
        parseFile(filePath);
    }

    ObjLoader::ObjLoader(const std::string &filePath,
                         const maths::Vector &scale,
                         const maths::Vector &center) {
        _transformScale = scale;
        _transformCenter = center;
        parseFile(filePath);
        applyTransformations();
        computeNormalsIfMissing();
    }

    void ObjLoader::parseFile(const std::string &filePath) {
        if (filePath.empty()) {
            throw exception::PluginException{"OBJ file path cannot be empty"};
        }
        std::ifstream file(filePath);
        if (!file.is_open()) {
            throw exception::PluginException{"Failed to open OBJ file: " +
                                             filePath};
        }
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') {
                continue;
            }
            processLine(line);
        }
        file.close();
    }

    void ObjLoader::processLine(const std::string &line) {
        std::istringstream iss(line);
        std::string keyword;
        iss >> keyword;

        auto it = _ops.find(keyword);
        if (it != _ops.end()) {
            it->second(*this, iss);
        }
    }

    maths::Vector ObjLoader::parseVector(std::istringstream &iss) const {
        double x, y, z;
        iss >> x >> y >> z;
        return maths::Vector(x, y, z);
    }

    void ObjLoader::handleVertex(ObjLoader &loader, std::istringstream &iss) {
        loader._vertices.emplace_back(loader.parseVector(iss));
    }

    void ObjLoader::handleNormal(ObjLoader &loader, std::istringstream &iss) {
        loader._normals.emplace_back(loader.parseVector(iss));
    }

    void ObjLoader::handleTexCoord(ObjLoader &loader, std::istringstream &iss) {
        double u, v;
        iss >> u >> v;
        loader._textureCoords.emplace_back(u, v, 0);
    }

    void ObjLoader::handleFace(ObjLoader &loader, std::istringstream &iss) {
        std::string v1, v2, v3;
        iss >> v1 >> v2 >> v3;
        const std::array<int, 3> face = {
            parseFaceVertex(v1), parseFaceVertex(v2), parseFaceVertex(v3)};
        int faceIndex = loader._faces.size();
        loader._faces.push_back({face[0], face[1], face[2]});
        loader._faceToMaterial[faceIndex] = loader._currentMaterialName;

        // Group face by material
        loader._groupsByMaterial[loader._currentMaterialName].push_back(
            faceIndex);
    }

    void ObjLoader::handleUseMtl(ObjLoader &loader, std::istringstream &iss) {
        iss >> loader._currentMaterialName;
    }

    int ObjLoader::parseFaceVertex(const std::string &token) {
        std::istringstream tokenStream(token);
        std::string vertexIndex;
        std::getline(tokenStream, vertexIndex, '/');
        return std::stoi(vertexIndex) - 1;
    }

    void ObjLoader::applyTransformations() {
        for (auto &vertex : _vertices) {
            vertex.x *= _transformScale.x;
            vertex.y *= _transformScale.y;
            vertex.z *= _transformScale.z;
        }
        for (auto &vertex : _vertices) {
            vertex += _transformCenter;
        }
    }

    void ObjLoader::computeNormalsIfMissing() {
        if (_normals.empty()) {
            _normals.resize(_vertices.size(), maths::Vector(0, 0, 0));
            for (const auto &face : _faces) {
                const maths::Vector &v0 = _vertices[face.v1];
                const maths::Vector &v1 = _vertices[face.v2];
                const maths::Vector &v2 = _vertices[face.v3];
                maths::Vector normal = (v1 - v0).cross(v2 - v0).normalized();
                _normals[face.v1] += normal;
                _normals[face.v2] += normal;
                _normals[face.v3] += normal;
            }
            for (auto &normal : _normals) {
                normal.normalize();
            }
        }
    }

}  // namespace raytracer::object::primitive
