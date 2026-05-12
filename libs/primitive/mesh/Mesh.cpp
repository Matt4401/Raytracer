/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Mesh
*/

#include "Mesh.hpp"

#include <any>
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

        const auto &faces = _objLoader->faces();
        const auto &vertices = _objLoader->vertices();
        _faces.reserve(faces.size());
        for (std::size_t i = 0; i < faces.size(); ++i) {
            const auto &f = faces[i];
            const maths::Vector v0 = vertices[f.fv1.v];
            const maths::Vector v1 = vertices[f.fv2.v];
            const maths::Vector v2 = vertices[f.fv3.v];
            auto face = std::make_shared<Face>(
                i, f, v0, v1, v2, _material,
                _objLoader->getMaterialForFace(static_cast<int>(i)));
            face->setId(static_cast<int>(i));
            _faces.push_back(face);
        }

        if (!_faces.empty()) {
            bvh::BVHBuilder<raytracer::bvh::ISplitStrategy> builder("sah");
            std::vector<std::shared_ptr<IPrimitive>> objs;
            objs.reserve(_faces.size());
            for (const auto &face : _faces) {
                objs.push_back(face);
            }
            _bvhRoot = builder.build(objs);
        }

        _lastHitTriangleIndex = std::nullopt;
    }

    SurfaceData Mesh::surfaceData(const maths::Vector &hitPoint) const {
        SurfaceData data{.normal = maths::Vector(0, 1, 0),
                         .uv = maths::Vector(0, 0, 0),
                         .extraParams = {},
                         .material = {}};
        if (!_lastHitTriangleIndex.has_value())
            return data;

        const std::size_t faceIndex = _lastHitTriangleIndex.value();
        if (faceIndex < _faces.size()) {
            return _faces[faceIndex]->surfaceData(hitPoint);
        }

        return data;
    }

    bool Mesh::hits(const maths::Ray &ray, HitRecord &record) const {
        if (_bvhRoot) {
            if (!_bvhRoot->hits(ray, record)) {
                _lastHitTriangleIndex = std::nullopt;
                return false;
            }
            if (record.objectId >= 0)
                _lastHitTriangleIndex =
                    static_cast<std::size_t>(record.objectId);
            else
                _lastHitTriangleIndex = std::nullopt;
            return true;
        }

        auto intersection = _surfaceHelper->findClosestTriangle(ray);
        if (!intersection) {
            _lastHitTriangleIndex = std::nullopt;
            return false;
        }
        _lastHitTriangleIndex =
            static_cast<std::size_t>(intersection->triangleIndex);
        record.t = intersection->distance;
        record.objectId = static_cast<int>(intersection->triangleIndex);
        return true;
    }

    IPrimitive::AABoundingBox Mesh::boundingBox() {
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

    void Mesh::buildTrianglesFromFaces() {
        for (const auto &face : _objLoader->faces()) {
            _surfaceHelper->addTriangle(face);
        }
    }
}  // namespace raytracer::object::primitive