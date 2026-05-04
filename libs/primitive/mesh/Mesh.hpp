/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Mesh
*/

#ifndef MESH_HPP_
#define MESH_HPP_

#include <any>
#include <map>
#include <memory>
#include <string>

#include "object/primitive/APrimitive.hpp"

namespace raytracer::object::primitive {
    class Mesh : public APrimitive {
      public:
        struct Triangle {
            int v1;
            int v2;
            int v3;
        };

        Mesh(const std::map<std::string, std::any> &params);
        ~Mesh() override = default;
        double hits(const maths::Ray &ray) override;
        BoundingBox boundingBox() override;
        SurfaceData surfaceData(const maths::Vector &hitPoint) const override;

      protected:
      private:
        BoundingBox triangleBoundingBox(const Triangle &triangle) const;
        double triangleHits(const maths::Ray &ray, int triangleIndex) const;
        maths::Vector triangleCentroid(const Triangle &triangle) const;
        void loadMesh(const std::string &filePath);

        void parseVertexLine(const std::string &line);
        void parseNormalLine(const std::string &line);
        void parseTexCoordLine(const std::string &line);
        void parseFaceLine(const std::string &line);
        void computeNormalsIfMissing();
        void applyScaleToVertices();
        void applyCenterToVertices();
        void buildTrianglesFromFaces();

        BoundingBox _meshBoundingBox{};

        maths::Vector _scale;
        std::vector<maths::Vector> _vertices;
        std::vector<std::array<int, 3>> _faces;
        std::vector<maths::Vector> _normals;
        std::vector<maths::Vector> _textureCoords;
        std::vector<Triangle> _triangles;
    };
}  // namespace raytracer::object::primitive
#endif /* !MESH_HPP_ */
