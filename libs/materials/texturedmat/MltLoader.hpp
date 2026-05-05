/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** MltLoader
*/

#pragma once

#include <functional>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "MeshMaterial.hpp"
#include "exception/PluginException.hpp"

namespace raytracer::object::material {
    class MltLoader {
      public:
        explicit MltLoader(const std::string &filePath);
        ~MltLoader() = default;

        const std::map<std::string, MeshMaterial> &materials() const {
            return _materials;
        }

        const MeshMaterial &get(const std::string &name) const;

      protected:
      private:
        static const std::unordered_map<
            std::string, std::function<void(MltLoader &, std::istringstream &,
                                            MeshMaterial &)>>
            _ops;

        void parseFile(const std::string &filePath);
        void processLine(const std::string &line, const std::string &filePath,
                         std::string &currentName, MeshMaterial &current);
        maths::Vector parseVector(std::istringstream &iss) const;
        void finishCurrentMaterial(std::string &currentName,
                                   MeshMaterial &current);
        void handleProperty(const std::string &keyword, std::istringstream &iss,
                            const std::string &filePath, MeshMaterial &current);

        static void handleNs(MltLoader &loader, std::istringstream &iss,
                             MeshMaterial &current);
        static void handleKa(MltLoader &loader, std::istringstream &iss,
                             MeshMaterial &current);
        static void handleKd(MltLoader &loader, std::istringstream &iss,
                             MeshMaterial &current);
        static void handleKs(MltLoader &loader, std::istringstream &iss,
                             MeshMaterial &current);
        static void handleKe(MltLoader &loader, std::istringstream &iss,
                             MeshMaterial &current);
        static void handleNi(MltLoader &loader, std::istringstream &iss,
                             MeshMaterial &current);
        static void handleD(MltLoader &loader, std::istringstream &iss,
                            MeshMaterial &current);
        static void handleIllum(MltLoader &loader, std::istringstream &iss,
                                MeshMaterial &current);
        static void handleMapKd(MltLoader &loader, std::istringstream &iss,
                                MeshMaterial &current);
        static void handleMapBump(MltLoader &loader, std::istringstream &iss,
                                  MeshMaterial &current);
        static std::string extractTexturePath(std::istringstream &iss);

        std::map<std::string, MeshMaterial> _materials;
    };

}  // namespace raytracer::object::material
