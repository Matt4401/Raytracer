/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** MeshGeometryGroup
*/

#ifndef MESHGEOMETRYGROUP_HPP_
#define MESHGEOMETRYGROUP_HPP_

#include <string>
#include <vector>

#include "math/Vector.hpp"

namespace raytracer::object::primitive {
    /**
     * @brief Represents a group of geometry (vertices, normals, texture coords,
     * faces) associated with a single material
     */
    struct MeshGeometryGroup {
        std::string materialName;
        std::vector<int> faceIndices;  // Indices into global faces array
        std::vector<maths::Vector> vertices;
        std::vector<maths::Vector> normals;
        std::vector<maths::Vector> textureCoords;

        MeshGeometryGroup() = default;
        explicit MeshGeometryGroup(const std::string &mtlName)
            : materialName(mtlName) {
        }
    };

}  // namespace raytracer::object::primitive

#endif /* !MESHGEOMETRYGROUP_HPP_ */
