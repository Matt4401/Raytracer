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
        _materialLoader = std::make_unique<MltLoader>(mtlPath);
        _surfaceHelper =
            std::make_unique<MeshSurfaceHelper>(_objLoader->vertices());

        buildTrianglesFromFaces();
    }

    double Mesh::hits(const maths::Ray &ray) {
        auto intersection = _surfaceHelper->findClosestTriangle(ray);
        return intersection ? intersection->distance : -1.0;
    }

    IPrimitive::BoundingBox Mesh::boundingBox() {
        if (_meshBoundingBox.w > 0 && _meshBoundingBox.h > 0 &&
            _meshBoundingBox.d > 0) {
            return _meshBoundingBox;
        }
        const auto &vertices = _objLoader->vertices();
        if (vertices.empty()) {
            return {0, 0, 0, 0, 0, 0};
        }
        double minX = vertices[0].x;
        double minY = vertices[0].y;
        double minZ = vertices[0].z;
        double maxX = vertices[0].x;
        double maxY = vertices[0].y;
        double maxZ = vertices[0].z;

        for (const auto &vertex : vertices) {
            if (vertex.x < minX)
                minX = vertex.x;
            if (vertex.y < minY)
                minY = vertex.y;
            if (vertex.z < minZ)
                minZ = vertex.z;
            if (vertex.x > maxX)
                maxX = vertex.x;
            if (vertex.y > maxY)
                maxY = vertex.y;
            if (vertex.z > maxZ)
                maxZ = vertex.z;
        }

        _meshBoundingBox = {minX,        minY,        minZ,
                            maxX - minX, maxY - minY, maxZ - minZ};
        return _meshBoundingBox;
    }

    SurfaceData Mesh::surfaceData(const maths::Vector &hitPoint) const {
        const auto &normals = _objLoader->normals();
        const auto &faceToMaterial = _objLoader->faceToMaterial();

        SurfaceData data{.normal = maths::Vector(0, 1, 0),
                         .uv = maths::Vector(0, 0, 0),
                         .extraParams = {},
                         .material = {}};

        int closestTriangleIdx = -1;
        double closestDist = std::numeric_limits<double>::infinity();

        const auto &triangles = _surfaceHelper->triangles();
        const auto &vertices = _objLoader->vertices();

        for (int i = 0; i < _surfaceHelper->triangleCount(); ++i) {
            const auto &tri = triangles[i];
            const maths::Vector &v0 = vertices[tri.v1];
            const maths::Vector &v1 = vertices[tri.v2];
            const maths::Vector &v2 = vertices[tri.v3];

            maths::Vector centroid = (v0 + v1 + v2) / 3.0;
            double dist = (hitPoint - centroid).magnitude();

            if (dist < closestDist) {
                closestDist = dist;
                closestTriangleIdx = i;
            }
        }

        if (closestTriangleIdx >= 0) {
            data.normal = _surfaceHelper->computeNormal(closestTriangleIdx,
                                                        hitPoint, normals);

            data.uv = MeshSurfaceHelper::computeUV(hitPoint, _center);

            auto it = faceToMaterial.find(closestTriangleIdx);
            if (it != faceToMaterial.end() && !it->second.empty()) {
                try {
                    const auto &meshMaterial = _materialLoader->get(it->second);
                    const auto kd = meshMaterial.Kd();
                    data.material.color = maths::Color(kd.x, kd.y, kd.z);
                    data.material.emission = meshMaterial.Ke();
                    data.material.transparency = meshMaterial.d();
                    data.material.ior = meshMaterial.Ni();
                    data.material.metalness = meshMaterial.Ns() / 1000.0;
                    data.material.reflectivity = 0.5;
                } catch (const std::exception &) {
                    throw exception::PluginException{
                        "Failed to load material '" + it->second +
                        "' for triangle " + std::to_string(closestTriangleIdx)};
                }
            }
        } else {
            data.normal = (hitPoint - _center);
            if (data.normal.magnitude() < K_RAY_EPSILON) {
                data.normal = maths::Vector(0, 1, 0);
            } else {
                data.normal = data.normal.normalized();
            }
        }

        if (this->_material) {
            MaterialProperties matProps =
                this->_material->evaluate(data, hitPoint);
            if (data.material.color.r < K_RAY_EPSILON && data.material.color.g < K_RAY_EPSILON &&
                data.material.color.b < K_RAY_EPSILON) {
                data.material = matProps;
            }
        }

        return data;
    }

    IPrimitive::BoundingBox Mesh::triangleBoundingBox(int triangleIndex) const {
        const auto &triangles = _surfaceHelper->triangles();
        if (triangleIndex < 0 ||
            triangleIndex >= static_cast<int>(triangles.size())) {
            return {0, 0, 0, 0, 0, 0};
        }

        const auto &vertices = _objLoader->vertices();
        const auto &tri = triangles[triangleIndex];
        const maths::Vector &v0 = vertices[tri.v1];
        const maths::Vector &v1 = vertices[tri.v2];
        const maths::Vector &v2 = vertices[tri.v3];

        const double minX = std::min({v0.x, v1.x, v2.x});
        const double minY = std::min({v0.y, v1.y, v2.y});
        const double minZ = std::min({v0.z, v1.z, v2.z});
        const double maxX = std::max({v0.x, v1.x, v2.x});
        const double maxY = std::max({v0.y, v1.y, v2.y});
        const double maxZ = std::max({v0.z, v1.z, v2.z});

        return {minX, minY, minZ, maxX - minX, maxY - minY, maxZ - minZ};
    }

    void Mesh::buildTrianglesFromFaces() {
        const auto &faces = _objLoader->faces();
        for (const auto &face : faces) {
            _surfaceHelper->addTriangle(face.v1, face.v2, face.v3);
        }
    }
}  // namespace raytracer::object::primitive