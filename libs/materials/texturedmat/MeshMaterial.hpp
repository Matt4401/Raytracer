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
        explicit MeshMaterial(const std::string &name, double Ns,
                              const maths::Vector &Ka, const maths::Vector &Ks,
                              const maths::Vector &Ke, const maths::Vector &Kd,
                              double Ni, double d, double illum)
            : _name(name),
              _Ns(Ns),
              _Ka(Ka),
              _Ks(Ks),
              _Ke(Ke),
              _Kd(Kd),
              _Ni(Ni),
              _d(d),
              _illum(illum) {};
        void setName(const std::string &name) {
            _name = name;
        }
        void setNs(double ns) {
            _Ns = ns;
        }
        void setKa(const maths::Vector &v) {
            _Ka = v;
        }
        void setKs(const maths::Vector &v) {
            _Ks = v;
        }
        void setKe(const maths::Vector &v) {
            _Ke = v;
        }
        void setKd(const maths::Vector &v) {
            _Kd = v;
        }
        void setMapKd(const std::string &path) {
            _mapKd = path;
        }
        void setMapBump(const std::string &path) {
            _mapBump = path;
        }
        void setNi(double ni) {
            _Ni = ni;
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
        double Ns() const {
            return _Ns;
        }
        const maths::Vector &Ka() const {
            return _Ka;
        }
        const maths::Vector &Ks() const {
            return _Ks;
        }
        const maths::Vector &Ke() const {
            return _Ke;
        }
        const maths::Vector &Kd() const {
            return _Kd;
        }
        const std::string &mapKd() const {
            return _mapKd;
        }
        const std::string &mapBump() const {
            return _mapBump;
        }
        double Ni() const {
            return _Ni;
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
        double _Ns;            // Specular exponent
        maths::Vector _Ka;     // Ambient color
        maths::Vector _Ks;     // Specular color
        maths::Vector _Ke;     // Emissive color
        maths::Vector _Kd;     // Diffuse color
        double _Ni;            // Optical density (index of refraction)
        double _d;             // Dissolve (transparency)
        double _illum;         // Illumination model
        std::string _mapKd;    // Diffuse texture path
        std::string _mapBump;  // Bump/normal map path
    };
}  // namespace raytracer::object::material
