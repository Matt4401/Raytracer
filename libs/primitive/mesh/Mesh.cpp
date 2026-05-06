/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Mesh
*/

#include "Mesh.hpp"

#include <iostream>
#include <limits>
#include <stdexcept>

#include "util/middleware/Helpers.hpp"
#include "util/middleware/ObjectMiddleware.hpp"

namespace raytracer::object::primitive {

    Mesh::Mesh(const std::map<std::string, std::any> &params)
        : APrimitive(
              "Mesh", util::Helpers::toVector(params, "center", "Mesh"),
              util::ObjectMiddleware::validate<
                  std::shared_ptr<raytracer::object::material::IMaterial>>(
                  params, "material", "Mesh")) {
        std::string objPath = util::ObjectMiddleware::validate<std::string>(
            params, "objPath", "Mesh");
        std::string mtlPath = util::ObjectMiddleware::validate<std::string>(
            params, "mtlPath", "Mesh");
        maths::Vector scale = util::Helpers::toVector(params, "scale", "Mesh");

        if (scale.x <= 0 || scale.y <= 0 || scale.z <= 0) {
            throw exception::PluginException{
                "Mesh scale components must be greater than zero"};
        }

        _objLoader = std::make_unique<ObjLoader>(objPath, scale, _center);
        _surfaceHelper =
            std::make_unique<MeshSurfaceHelper>(_objLoader->vertices());
        buildTrianglesFromFaces();
        _lastHitTriangleIndex = std::nullopt;
    }

    double Mesh::hits(const maths::Ray &ray) {
        auto intersection = _surfaceHelper->findClosestTriangle(ray);
        if (intersection) {
            _lastHitTriangleIndex = intersection;
            return intersection->distance;
        }
        _lastHitTriangleIndex = std::nullopt;
        return -1.0;
    }

    IPrimitive::BoundingBox Mesh::boundingBox() {
        if (_meshBoundingBox.w > 0 && _meshBoundingBox.h > 0 &&
            _meshBoundingBox.d > 0) {
            return _meshBoundingBox;
        }
        const auto &vertices = _objLoader->vertices();
        if (vertices.empty())
            return {0, 0, 0, 0, 0, 0};

        double minX = vertices[0].x, minY = vertices[0].y, minZ = vertices[0].z;
        double maxX = vertices[0].x, maxY = vertices[0].y, maxZ = vertices[0].z;

        for (const auto &v : vertices) {
            if (v.x < minX)
                minX = v.x;
            if (v.y < minY)
                minY = v.y;
            if (v.z < minZ)
                minZ = v.z;
            if (v.x > maxX)
                maxX = v.x;
            if (v.y > maxY)
                maxY = v.y;
            if (v.z > maxZ)
                maxZ = v.z;
        }

        _meshBoundingBox = {minX,        minY,        minZ,
                            maxX - minX, maxY - minY, maxZ - minZ};
        return _meshBoundingBox;
    }

    SurfaceData Mesh::surfaceData(const maths::Vector &hitPoint) const {
        SurfaceData data{.normal = maths::Vector(0, 1, 0),
                         .uv = maths::Vector(0, 0, 0),
                         .extraParams = {},
                         .material = {}};
        if (!_lastHitTriangleIndex.has_value())
            return data;
        int closestIdx = _lastHitTriangleIndex.value().triangleIndex;
        data.normal = _surfaceHelper->computeNormal(closestIdx, hitPoint,
                                                    _objLoader->normals());
        data.uv = _surfaceHelper->computeUV(closestIdx, hitPoint,
                                            _objLoader->textureCoords());

        data.extraParams["materialName"] =
            _objLoader->getMaterialForFace(closestIdx);

        if (this->_material)
            this->_material->evaluate(data, hitPoint);
        return data;
    }

    IPrimitive::BoundingBox Mesh::triangleBoundingBox(int triangleIndex) const {
        const auto &triangles = _surfaceHelper->triangles();
        if (triangleIndex < 0 ||
            triangleIndex >= static_cast<int>(triangles.size()))
            return {0, 0, 0, 0, 0, 0};

        const auto &vertices = _objLoader->vertices();
        const auto &face = triangles[triangleIndex];
        const maths::Vector &v0 = vertices[face.fv1.v];
        const maths::Vector &v1 = vertices[face.fv2.v];
        const maths::Vector &v2 = vertices[face.fv3.v];

        const double minX = std::min({v0.x, v1.x, v2.x});
        const double minY = std::min({v0.y, v1.y, v2.y});
        const double minZ = std::min({v0.z, v1.z, v2.z});
        const double maxX = std::max({v0.x, v1.x, v2.x});
        const double maxY = std::max({v0.y, v1.y, v2.y});
        const double maxZ = std::max({v0.z, v1.z, v2.z});

        return {minX, minY, minZ, maxX - minX, maxY - minY, maxZ - minZ};
    }

    void Mesh::buildTrianglesFromFaces() {
        for (const auto &face : _objLoader->faces()) {
            _surfaceHelper->addTriangle(face);
        }
    }
}  // namespace raytracer::object::primitive