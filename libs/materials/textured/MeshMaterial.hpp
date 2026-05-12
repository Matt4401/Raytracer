/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** MeshMaterial
*/

#pragma once

#include <string>

#include "math/Vector.hpp"

namespace raytracer::object::material {
    class MeshMaterial {
      public:
        MeshMaterial() = default;
        explicit MeshMaterial(const std::string &name, double ns,
                              const maths::Vector &ka, const maths::Vector &ks,
                              const maths::Vector &ke, const maths::Vector &kd,
                              double ni, double d, double illum)
            : _name(name),
              _ns(ns),
              _ka(ka),
              _ks(ks),
              _ke(ke),
              _kd(kd),
              _ni(ni),
              _d(d),
              _illum(illum) {};
        void setName(const std::string &name) {
            _name = name;
        }
        void setNs(double ns) {
            _ns = ns;
        }
        void setKa(const maths::Vector &v) {
            _ka = v;
        }
        void setKs(const maths::Vector &v) {
            _ks = v;
        }
        void setKe(const maths::Vector &v) {
            _ke = v;
        }
        void setKd(const maths::Vector &v) {
            _kd = v;
        }
        void setMapKd(const std::string &path) {
            _mapKd = path;
        }
        void setMapBump(const std::string &path) {
            _mapBump = path;
        }
        void setNi(double ni) {
            _ni = ni;
        }
        void setD(double d) {
            _d = d;
        }
        void setIllum(double illum) {
            _illum = illum;
        }

        const std::string &name() const {
            return _name;
        }
        double ns() const {
            return _ns;
        }
        const maths::Vector &ka() const {
            return _ka;
        }
        const maths::Vector &ks() const {
            return _ks;
        }
        const maths::Vector &ke() const {
            return _ke;
        }
        const maths::Vector &kd() const {
            return _kd;
        }
        const std::string &mapKd() const {
            return _mapKd;
        }
        const std::string &mapBump() const {
            return _mapBump;
        }
        double ni() const {
            return _ni;
        }
        double d() const {
            return _d;
        }
        double illum() const {
            return _illum;
        }
        ~MeshMaterial() = default;

      protected:
      private:
        std::string _name;     // Material name
        double _ns;            // Specular exponent
        maths::Vector _ka;     // Ambient color
        maths::Vector _ks;     // Specular color
        maths::Vector _ke;     // Emissive color
        maths::Vector _kd;     // Diffuse color
        double _ni;            // Optical density (index of refraction)
        double _d;             // Dissolve (transparency)
        double _illum;         // Illumination model
        std::string _mapKd;    // Diffuse texture path
        std::string _mapBump;  // Bump/normal map path
    };
}  // namespace raytracer::object::material
