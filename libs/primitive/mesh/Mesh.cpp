/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Mesh
*/

#include "Mesh.hpp"

#include <any>
#include <cmath>
#include <memory>
#include <vector>

#include "bvh/BVHBuilder.hpp"
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
        maths::Vector scale = util::Helpers::toVector(params, "scale", "Mesh");

        if (scale.x <= 0 || scale.y <= 0 || scale.z <= 0) {
            throw exception::PluginException{
                "Mesh scale components must be greater than zero"};
        }

        _objLoader = std::make_unique<ObjLoader>(objPath, scale, _center);
        _surfaceHelper = std::make_unique<MeshSurfaceHelper>(
            _objLoader->vertices(), _objLoader->faces());

        if (_objLoader->vertices().empty() || _objLoader->faces().empty()) {
            throw exception::PluginException{
                "Mesh must have at least one vertex and one face"};
        }
        _faces.reserve(_objLoader->faces().size());
        for (std::size_t i = 0; i < _objLoader->faces().size(); ++i) {
            const auto &f = _objLoader->faces()[i];
            const maths::Vector v0 = _objLoader->vertices()[f.fv1.v];
            const maths::Vector v1 = _objLoader->vertices()[f.fv2.v];
            const maths::Vector v2 = _objLoader->vertices()[f.fv3.v];
            auto face = std::make_shared<Face>(
                i, f, v0, v1, v2, _material,
                _objLoader->getMaterialForFace(static_cast<int>(i)));
            face->setId(static_cast<int>(i));
            _faces.push_back(face);
        }

        if (!_faces.empty()) {
            bvh::BVHBuilder<raytracer::bvh::ISplitStrategy> builder("sah");
            _bvhRoot = builder.build(_faces);
        }
    }

    SurfaceData Mesh::surfaceData(const HitRecord &record) const {
        SurfaceData data{.normal = maths::Vector(0, 1, 0),
                         .uv = maths::Vector(0, 0, 0),
                         .extraParams = {},
                         .material = {}};

        const int triangleIndex = record.triangleIndex;
        if (triangleIndex < 0 ||
            triangleIndex >= static_cast<int>(_faces.size())) {
            return data;
        }

        return _faces[static_cast<std::size_t>(triangleIndex)]->surfaceData(
            record);
    }

    bool Mesh::hits(const maths::Ray &ray, HitRecord &record) const {
        if (_bvhRoot) {
            if (!_bvhRoot->hits(ray, record)) {
                return false;
            }
            record.hitPoint = ray.origin + ray.direction * record.t;
            record.triangleIndex = record.objectId;
            record.objectId = getId();
            return true;
        }

        auto intersection = _surfaceHelper->findClosestTriangle(ray);
        if (!intersection) {
            return false;
        }
        record.t = intersection->distance;
        record.triangleIndex = intersection->triangleIndex;
        record.hitPoint = intersection->hitPoint;
        record.objectId = getId();
        return true;
    }

    IPrimitive::AABoundingBox Mesh::boundingBox() {
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
}  // namespace raytracer::object::primitive