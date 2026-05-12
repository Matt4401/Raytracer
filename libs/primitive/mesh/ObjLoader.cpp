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
            if (line.empty() || line[0] == '#')
                continue;
            processLine(line);
        }
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
        std::string t1, t2, t3;
        iss >> t1 >> t2 >> t3;

        int faceIndex = static_cast<int>(loader._faces.size());
        loader._faces.push_back(
            {parseFaceVertex(t1), parseFaceVertex(t2), parseFaceVertex(t3)});
        loader._faceToMaterial[faceIndex] = loader._currentMaterialName;
        loader._groupsByMaterial[loader._currentMaterialName].push_back(
            faceIndex);
    }

    void ObjLoader::handleUseMtl(ObjLoader &loader, std::istringstream &iss) {
        iss >> loader._currentMaterialName;
    }

    ObjLoader::FaceVertex ObjLoader::parseFaceVertex(const std::string &token) {
        FaceVertex fv;
        std::istringstream ss(token);
        std::string part;

        if (std::getline(ss, part, '/') && !part.empty())
            fv.v = std::stoi(part) - 1;

        if (std::getline(ss, part, '/')) {
            if (!part.empty())
                fv.vt = std::stoi(part) - 1;
        } else {
            return fv;
        }
        if (std::getline(ss, part, '/') && !part.empty())
            fv.vn = std::stoi(part) - 1;
        return fv;
    }

    void ObjLoader::applyTransformations() {
        for (auto &vertex : _vertices) {
            vertex.x *= _transformScale.x;
            vertex.y *= _transformScale.y;
            vertex.z *= _transformScale.z;
            vertex += _transformCenter;
        }
    }

    void ObjLoader::computeNormalsIfMissing() {
        if (!_normals.empty())
            return;

        _normals.resize(_vertices.size(), maths::Vector(0, 0, 0));
        for (const auto &face : _faces) {
            const maths::Vector &v0 = _vertices[face.fv1.v];
            const maths::Vector &v1 = _vertices[face.fv2.v];
            const maths::Vector &v2 = _vertices[face.fv3.v];
            maths::Vector normal = (v1 - v0).cross(v2 - v0).normalized();
            _normals[face.fv1.v] += normal;
            _normals[face.fv2.v] += normal;
            _normals[face.fv3.v] += normal;
        }
        for (auto &normal : _normals) normal.normalize();
    }

}  // namespace raytracer::object::primitive