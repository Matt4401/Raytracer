/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Mesh
*/

#include "Mesh.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
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
        _scale = util::Helpers::toVector(params, "scale", "Mesh");
        if (_scale.x <= 0 || _scale.y <= 0 || _scale.z <= 0) {
            throw exception::PluginException{
                "Mesh scale components must be greater than zero"};
        }
        loadMesh(util::ObjectMiddleware::validate<std::string>(params, "path",
                                                               "Mesh"));
    }

    double Mesh::hits(const maths::Ray &ray) {
        double closestT = std::numeric_limits<double>::infinity();
        for (size_t i = 0; i < _triangles.size(); ++i) {
            double t = triangleHits(ray, i);
            if (t > 0 && t < closestT) {
                closestT = t;
            }
        }
        return closestT == std::numeric_limits<double>::infinity() ? -1.0
                                                                   : closestT;
    }

    IPrimitive::BoundingBox Mesh::boundingBox() {
        if (_meshBoundingBox.w > 0 && _meshBoundingBox.h > 0 &&
            _meshBoundingBox.d > 0) {
            return _meshBoundingBox;
        }
        if (_vertices.empty()) {
            return {0, 0, 0, 0, 0, 0};
        }
        double minX = _vertices[0].x;
        double minY = _vertices[0].y;
        double minZ = _vertices[0].z;
        double maxX = _vertices[0].x;
        double maxY = _vertices[0].y;
        double maxZ = _vertices[0].z;

        for (const auto &vertex : _vertices) {
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
        maths::Vector normal(0, 0, 0);
        double bestDistance = std::numeric_limits<double>::infinity();
        for (const auto &triangle : _triangles) {
            const maths::Vector &v0 = _vertices[triangle.v1];
            const maths::Vector &v1 = _vertices[triangle.v2];
            const maths::Vector &v2 = _vertices[triangle.v3];
            const maths::Vector triNormal = (v1 - v0).cross(v2 - v0);
            const double triMagnitude = triNormal.magnitude();
            if (triMagnitude < kRayEpsilon) {
                continue;
            }

            const maths::Vector unitNormal = triNormal / triMagnitude;
            const double distance = std::fabs((hitPoint - v0).dot(unitNormal));
            if (distance < bestDistance) {
                bestDistance = distance;
                normal = unitNormal;
            }
        }

        if (normal.magnitude() < kRayEpsilon) {
            normal = (hitPoint - _center);
            if (normal.magnitude() < kRayEpsilon) {
                normal = maths::Vector(0, 1, 0);
            } else {
                normal = normal.normalized();
            }
        }

        double u = 0, v = 0;
        u = (hitPoint.x - _center.x) / _scale.x;
        v = (hitPoint.y - _center.y) / _scale.y;

        SurfaceData data{
            .normal = normal, .uv = maths::Vector(u, v, 0), .material = {}};

        if (this->_material) {
            data.material = this->_material->evaluate(data, hitPoint);
        }
        return data;
    }

    IPrimitive::BoundingBox Mesh::triangleBoundingBox(
        const Triangle &triangle) const {
        const maths::Vector &v0 = _vertices[triangle.v1];
        const maths::Vector &v1 = _vertices[triangle.v2];
        const maths::Vector &v2 = _vertices[triangle.v3];

        const double minX = std::min({v0.x, v1.x, v2.x});
        const double minY = std::min({v0.y, v1.y, v2.y});
        const double minZ = std::min({v0.z, v1.z, v2.z});
        const double maxX = std::max({v0.x, v1.x, v2.x});
        const double maxY = std::max({v0.y, v1.y, v2.y});
        const double maxZ = std::max({v0.z, v1.z, v2.z});

        return {minX, minY, minZ, maxX - minX, maxY - minY, maxZ - minZ};
    }

    double Mesh::triangleHits(const maths::Ray &ray, int triangleIndex) const {
        Triangle triangle = _triangles.at(triangleIndex);
        float t, u, v = 0.0f;
        const maths::Vector &v0 = _vertices[triangle.v1];
        const maths::Vector &v1 = _vertices[triangle.v2];
        const maths::Vector &v2 = _vertices[triangle.v3];
        const maths::Vector edge1 = v1 - v0;
        const maths::Vector edge2 = v2 - v0;
        const maths::Vector h = ray.direction.cross(edge2);
        const double a = edge1.dot(h);
        if (std::fabs(a) < kRayEpsilon)
            return -1.0;
        const double f = 1.0 / a;
        const maths::Vector s = ray.origin - v0;
        u = f * s.dot(h);
        if (u < 0.0 || u > 1.0)
            return -1.0;
        const maths::Vector q = s.cross(edge1);
        v = f * ray.direction.dot(q);
        if (v < 0.0 || u + v > 1.0)
            return -1.0;
        t = f * edge2.dot(q);
        if (t > kRayEpsilon)
            return t;
        else
            return -1.0;
        return -1.0;
    }

    maths::Vector Mesh::triangleCentroid(const Triangle &triangle) const {
        const maths::Vector &v0 = _vertices[triangle.v1];
        const maths::Vector &v1 = _vertices[triangle.v2];
        const maths::Vector &v2 = _vertices[triangle.v3];
        return (v0 + v1 + v2) / 3.0;
    }

    void Mesh::parseVertexLine(const std::string &line) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;
        double x, y, z;
        iss >> x >> y >> z;
        _vertices.emplace_back(x, y, z);
    }

    void Mesh::parseNormalLine(const std::string &line) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;
        double x, y, z;
        iss >> x >> y >> z;
        _normals.emplace_back(x, y, z);
    }

    void Mesh::parseTexCoordLine(const std::string &line) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;
        double u, v;
        iss >> u >> v;
        _textureCoords.emplace_back(u, v, 0);
    }

    void Mesh::parseFaceLine(const std::string &line) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        auto parseFaceVertex = [](const std::string &token) {
            std::istringstream tokenStream(token);
            std::string vertexIndex;
            std::getline(tokenStream, vertexIndex, '/');
            return std::stoi(vertexIndex) - 1;
        };

        std::string v1;
        std::string v2;
        std::string v3;
        iss >> v1 >> v2 >> v3;
        const std::array<int, 3> face = {
            parseFaceVertex(v1), parseFaceVertex(v2), parseFaceVertex(v3)};
        std::cerr << "[Mesh] face: " << face[0] << ", " << face[1] << ", "
                  << face[2] << std::endl;
        _faces.push_back(face);
    }

    void Mesh::computeNormalsIfMissing() {
        if (_normals.empty()) {
            _normals.resize(_vertices.size(), maths::Vector(0, 0, 0));
            for (const auto &face : _faces) {
                const maths::Vector &v0 = _vertices[face[0]];
                const maths::Vector &v1 = _vertices[face[1]];
                const maths::Vector &v2 = _vertices[face[2]];
                maths::Vector normal = (v1 - v0).cross(v2 - v0).normalized();
                _normals[face[0]] += normal;
                _normals[face[1]] += normal;
                _normals[face[2]] += normal;
            }
            for (auto &normal : _normals) {
                normal.normalize();
            }
        }
    }

    void Mesh::applyScaleToVertices() {
        for (auto &vertex : _vertices) {
            vertex.x *= _scale.x;
            vertex.y *= _scale.y;
            vertex.z *= _scale.z;
        }
    }

    void Mesh::applyCenterToVertices() {
        for (auto &vertex : _vertices) {
            vertex += _center;
        }
    }

    void Mesh::buildTrianglesFromFaces() {
        _triangles.reserve(_faces.size());
        for (const auto &face : _faces) {
            _triangles.push_back({face[0], face[1], face[2]});
        }
    }

    void Mesh::loadMesh(const std::string &filePath) {
        if (filePath.empty()) {
            throw exception::PluginException{"Mesh file path cannot be empty"};
        }
        std::ifstream file(filePath);
        if (!file.is_open()) {
            throw exception::PluginException{"Failed to open Mesh file: " +
                                             filePath};
        }
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty())
                continue;
            if (line.rfind("v ", 0) == 0) {
                parseVertexLine(line);
            } else if (line.rfind("vn ", 0) == 0) {
                parseNormalLine(line);
            } else if (line.rfind("vt ", 0) == 0) {
                parseTexCoordLine(line);
            } else if (line.rfind("f ", 0) == 0) {
                parseFaceLine(line);
            }
        }
        computeNormalsIfMissing();
        applyScaleToVertices();
        applyCenterToVertices();
        buildTrianglesFromFaces();
        file.close();
    }
}  // namespace raytracer::object::primitive