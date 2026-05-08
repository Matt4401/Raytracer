/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** MtlLoader
*/

#pragma once

#include <functional>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>

#include "MeshMaterial.hpp"

namespace raytracer::object::material {
    class MtlLoader {
      public:
        explicit MtlLoader(const std::string &filePath);
        ~MtlLoader() = default;

        const std::map<std::string, MeshMaterial> &materials() const {
            return _materials;
        }

        const MeshMaterial &get(const std::string &name) const;

      protected:
      private:
        static const std::unordered_map<
            std::string, std::function<void(MtlLoader &, std::istringstream &,
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

        static void handleNs(MtlLoader &loader, std::istringstream &iss,
                             MeshMaterial &current);
        static void handleKa(MtlLoader &loader, std::istringstream &iss,
                             MeshMaterial &current);
        static void handleKd(MtlLoader &loader, std::istringstream &iss,
                             MeshMaterial &current);
        static void handleKs(MtlLoader &loader, std::istringstream &iss,
                             MeshMaterial &current);
        static void handleKe(MtlLoader &loader, std::istringstream &iss,
                             MeshMaterial &current);
        static void handleNi(MtlLoader &loader, std::istringstream &iss,
                             MeshMaterial &current);
        static void handleD(MtlLoader &loader, std::istringstream &iss,
                            MeshMaterial &current);
        static void handleIllum(MtlLoader &loader, std::istringstream &iss,
                                MeshMaterial &current);
        static void handleMapKd(MtlLoader &loader, std::istringstream &iss,
                                MeshMaterial &current);
        static void handleMapBump(MtlLoader &loader, std::istringstream &iss,
                                  MeshMaterial &current);
        static std::string extractTexturePath(std::istringstream &iss);

        std::map<std::string, MeshMaterial> _materials;
    };

}  // namespace raytracer::object::material
