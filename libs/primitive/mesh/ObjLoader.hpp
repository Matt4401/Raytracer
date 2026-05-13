/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** ObjLoader
*/

#pragma once

#include <functional>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "Face.hpp"
#include "exception/PluginException.hpp"
#include "math/Vector.hpp"

namespace raytracer::object::material {
    class IMaterial;
}
namespace raytracer::object::primitive {
    class IPrimitive;
    class ObjLoader {
      public:
        explicit ObjLoader(
            const std::string &filePath,
            const std::shared_ptr<raytracer::object::material::IMaterial>
                &defaultMaterial = nullptr);
        explicit ObjLoader(
            const std::string &filePath, const maths::Vector &scale,
            const maths::Vector &center,
            const std::shared_ptr<raytracer::object::material::IMaterial>
                &defaultMaterial = nullptr);
        ~ObjLoader() = default;

        const std::vector<std::shared_ptr<IPrimitive>> &primitives() const {
            return _primitives;
        }

        struct ParsedFaceVertex {
            int v = -1;
            int vt = -1;
            int vn = -1;
        };

      private:
        static const std::unordered_map<
            std::string, std::function<void(ObjLoader &, std::istringstream &)>>
            _ops;

        void parseFile(const std::string &filePath);
        void processLine(const std::string &line);
        maths::Vector parseVector(std::istringstream &iss) const;

        static ParsedFaceVertex parseFaceVertex(const std::string &token);
        static void handleVertex(ObjLoader &loader, std::istringstream &iss);
        static void handleNormal(ObjLoader &loader, std::istringstream &iss);
        static void handleTexCoord(ObjLoader &loader, std::istringstream &iss);
        static void handleFace(ObjLoader &loader, std::istringstream &iss);
        static void handleUseMtl(ObjLoader &loader, std::istringstream &iss);
        int materialIndexForCurrent();

        std::shared_ptr<Face::MeshBuffers> _buffers =
            std::make_shared<Face::MeshBuffers>();
        std::vector<std::shared_ptr<IPrimitive>> _primitives;
        std::string _currentMaterialName;
        std::unordered_map<std::string, int> _materialNameToIndex;
        std::shared_ptr<raytracer::object::material::IMaterial>
            _defaultMaterial;
        maths::Vector _transformScale{1, 1, 1};
        maths::Vector _transformCenter{0, 0, 0};
    };

}  // namespace raytracer::object::primitive
