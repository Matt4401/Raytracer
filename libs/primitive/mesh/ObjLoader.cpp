/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** ObjLoader
*/

#include "ObjLoader.hpp"

#include <fstream>

#include "Face.hpp"

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

    static maths::Vector transformPoint(const maths::Vector &point,
                                        const maths::Vector &scale,
                                        const maths::Vector &center) {
        maths::Vector transformed(point);
        transformed.x *= scale.x;
        transformed.y *= scale.y;
        transformed.z *= scale.z;
        transformed += center;
        return transformed;
    }

    ObjLoader::ObjLoader(
        const std::string &filePath,
        std::shared_ptr<raytracer::object::material::IMaterial> defaultMaterial)
        : _defaultMaterial(defaultMaterial) {
        parseFile(filePath);
    }

    ObjLoader::ObjLoader(
        const std::string &filePath, const maths::Vector &scale,
        const maths::Vector &center,
        std::shared_ptr<raytracer::object::material::IMaterial> defaultMaterial)
        : _defaultMaterial(defaultMaterial) {
        _transformScale = scale;
        _transformCenter = center;
        parseFile(filePath);
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
        loader._buffers->vertices.emplace_back(
            transformPoint(loader.parseVector(iss), loader._transformScale,
                           loader._transformCenter));
    }

    void ObjLoader::handleNormal(ObjLoader &loader, std::istringstream &iss) {
        loader._buffers->normals.emplace_back(loader.parseVector(iss));
    }

    void ObjLoader::handleTexCoord(ObjLoader &loader, std::istringstream &iss) {
        double u, v;
        iss >> u >> v;
        loader._buffers->texCoords.emplace_back(u, v, 0);
    }

    ObjLoader::ParsedFaceVertex ObjLoader::parseFaceVertex(
        const std::string &token) {
        ParsedFaceVertex fv;
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

    void ObjLoader::handleFace(ObjLoader &loader, std::istringstream &iss) {
        std::vector<std::string> tokens;
        std::string tok;
        while (iss >> tok) {
            tokens.push_back(tok);
        }

        if (tokens.size() < 3)
            return;

        // fan triangulation: first vertex + (i, i+1)
        const ParsedFaceVertex fv0 = parseFaceVertex(tokens[0]);
        if (fv0.v < 0 ||
            fv0.v >= static_cast<int>(loader._buffers->vertices.size())) {
            return;
        }
        for (std::size_t i = 1; i + 1 < tokens.size(); ++i) {
            const ParsedFaceVertex fv1 = parseFaceVertex(tokens[i]);
            const ParsedFaceVertex fv2 = parseFaceVertex(tokens[i + 1]);
            if (fv1.v < 0 || fv2.v < 0 ||
                fv1.v >= static_cast<int>(loader._buffers->vertices.size()) ||
                fv2.v >= static_cast<int>(loader._buffers->vertices.size())) {
                continue;
            }

            Face::IndexedTriangle tri;
            tri.v0 = fv0.v;
            tri.v1 = fv1.v;
            tri.v2 = fv2.v;
            tri.vt0 = fv0.vt;
            tri.vt1 = fv1.vt;
            tri.vt2 = fv2.vt;
            tri.vn0 = fv0.vn;
            tri.vn1 = fv1.vn;
            tri.vn2 = fv2.vn;
            tri.materialIndex = loader.materialIndexForCurrent();

            auto prim = std::make_shared<raytracer::object::primitive::Face>(
                loader._primitives.size(), loader._buffers, tri,
                loader._defaultMaterial);
            prim->setId(static_cast<int>(loader._primitives.size()));
            loader._primitives.push_back(prim);
        }
    }

    void ObjLoader::handleUseMtl(ObjLoader &loader, std::istringstream &iss) {
        iss >> loader._currentMaterialName;
    }

    int ObjLoader::materialIndexForCurrent() {
        if (_currentMaterialName.empty()) {
            return -1;
        }
        const auto it = _materialNameToIndex.find(_currentMaterialName);
        if (it != _materialNameToIndex.end()) {
            return it->second;
        }
        const int nextIndex = static_cast<int>(_buffers->materialNames.size());
        _buffers->materialNames.push_back(_currentMaterialName);
        _materialNameToIndex.emplace(_currentMaterialName, nextIndex);
        return nextIndex;
    }

}  // namespace raytracer::object::primitive