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

        ObjLoader objLoader(objPath, scale, _center, _material);
        _faces = objLoader.primitives();

        if (_faces.empty()) {
            throw exception::PluginException{
                "Mesh must have at least one vertex and one face"};
        }

        raytracer::bvh::BVHBuilder<raytracer::bvh::ISplitStrategy> builder(
            "sah");
        _bvhRoot = builder.build(_faces);
    }

    SurfaceData Mesh::surfaceData(const HitRecord &record) const {
        SurfaceData data{.normal = maths::Vector(0, 1, 0),
                         .uv = maths::Vector(0, 0, 0),
                         .materialName = std::string(),
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
        if (!_bvhRoot) {
            return false;
        }
        if (!_bvhRoot->hits(ray, record)) {
            return false;
        }

        record.hitPoint = ray.origin + ray.direction * record.t;
        record.triangleIndex = record.objectId;
        record.objectId = getId();
        return true;
    }

    IPrimitive::AABoundingBox Mesh::boundingBox() {
        if (_faces.empty())
            return {0, 0, 0, 0, 0, 0};

        auto bounds = _faces.front()->boundingBox();
        double minX = bounds.x;
        double minY = bounds.y;
        double minZ = bounds.z;
        double maxX = bounds.x + bounds.w;
        double maxY = bounds.y + bounds.h;
        double maxZ = bounds.z + bounds.d;

        for (const auto &face : _faces) {
            bounds = face->boundingBox();
            minX = std::min(minX, bounds.x);
            minY = std::min(minY, bounds.y);
            minZ = std::min(minZ, bounds.z);
            maxX = std::max(maxX, bounds.x + bounds.w);
            maxY = std::max(maxY, bounds.y + bounds.h);
            maxZ = std::max(maxZ, bounds.z + bounds.d);
        }

        _meshBoundingBox = {minX,        minY,        minZ,
                            maxX - minX, maxY - minY, maxZ - minZ};
        return _meshBoundingBox;
    }
}  // namespace raytracer::object::primitive