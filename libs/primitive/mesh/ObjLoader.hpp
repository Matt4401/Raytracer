/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** ObjLoader
*/

#pragma once

#include <array>
#include <functional>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "exception/PluginException.hpp"
#include "math/Vector.hpp"

namespace raytracer::object::primitive {
    class ObjLoader {
      public:
        struct FaceVertex {
            int v = -1;
            int vt = -1;
            int vn = -1;
        };

        struct Face {
            FaceVertex fv1;
            FaceVertex fv2;
            FaceVertex fv3;
        };

        explicit ObjLoader(const std::string &filePath);
        explicit ObjLoader(const std::string &filePath,
                           const maths::Vector &scale,
                           const maths::Vector &center);
        ~ObjLoader() = default;

        const std::vector<maths::Vector> &vertices() const {
            return _vertices;
        }
        const std::vector<maths::Vector> &normals() const {
            return _normals;
        }
        const std::vector<maths::Vector> &textureCoords() const {
            return _textureCoords;
        }
        const std::vector<Face> &faces() const {
            return _faces;
        }
        const std::string &currentMaterialName() const {
            return _currentMaterialName;
        }
        const std::map<int, std::string> &faceToMaterial() const {
            return _faceToMaterial;
        }
        const maths::Vector &scale() const {
            return _scale;
        }
        const std::map<std::string, std::vector<int>> &groupsByMaterial()
            const {
            return _groupsByMaterial;
        }

        std::string getMaterialForFace(int faceIndex) const {
            auto it = _faceToMaterial.find(faceIndex);
            return (it != _faceToMaterial.end()) ? it->second : "";
        }

      private:
        static const std::unordered_map<
            std::string, std::function<void(ObjLoader &, std::istringstream &)>>
            _ops;

        void parseFile(const std::string &filePath);
        void processLine(const std::string &line);
        maths::Vector parseVector(std::istringstream &iss) const;
        void applyTransformations();
        void computeNormalsIfMissing();

        static void handleVertex(ObjLoader &loader, std::istringstream &iss);
        static void handleNormal(ObjLoader &loader, std::istringstream &iss);
        static void handleTexCoord(ObjLoader &loader, std::istringstream &iss);
        static void handleFace(ObjLoader &loader, std::istringstream &iss);
        static void handleUseMtl(ObjLoader &loader, std::istringstream &iss);
        static FaceVertex parseFaceVertex(const std::string &token);

        std::vector<maths::Vector> _vertices;
        std::vector<maths::Vector> _normals;
        std::vector<maths::Vector> _textureCoords;
        std::vector<Face> _faces;
        std::map<int, std::string> _faceToMaterial;
        std::map<std::string, std::vector<int>> _groupsByMaterial;
        std::string _currentMaterialName;
        maths::Vector _scale;
        maths::Vector _transformScale{1, 1, 1};
        maths::Vector _transformCenter{0, 0, 0};
    };

}  // namespace raytracer::object::primitive
