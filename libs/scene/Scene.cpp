/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Scene
*/

#include "Scene.hpp"

#include <algorithm>

#include "object/ACamera.hpp"
#include "object/ALight.hpp"
#include "object/AObject.hpp"
#include "object/IScene.hpp"
#include "object/primitive/IPrimitive.hpp"
#include "object/primitive/ReflTypes.hpp"
#include "util/middleware/ObjectMiddleware.hpp"

namespace raytracer::object::scene {
    Scene::Scene() : AScene() {
    }

    Scene::Scene(const std::map<std::string, std::any> &params)
        : AScene(params) {
    }

    void Scene::buildONB(const maths::Vector &w, maths::Vector &u,
                         maths::Vector &v) const {
        u = ((std::fabs(w.x) > 0.1 ? maths::Vector(0, 1, 0)
                                   : maths::Vector(1, 0, 0))
                 .cross(w))
                .normalized();
        v = w.cross(u);
    }

    maths::Vector Scene::randomCosineDir(const maths::Vector &nl,
                                         unsigned short *Xi) const {
        double r1 = 2 * M_PI * ::erand48(Xi);
        double r2 = ::erand48(Xi);
        double r2s = std::sqrt(r2);
        maths::Vector u, v;
        buildONB(nl, u, v);
        return (u * std::cos(r1) * r2s + v * std::sin(r1) * r2s +
                nl * std::sqrt(1 - r2))
            .normalized();
    }

    bool Scene::intersect(const maths::Ray &ray, double &t,
                          int &objectId) const {
        const double infinity = std::numeric_limits<double>::infinity();
        t = infinity;
        objectId = -1;
        for (size_t i = 0; i < _primitives.size(); ++i) {
            const double distance = _primitives.at(i)->hits(ray);
            if (distance >= 0.0 && distance < t) {
                t = distance;
                objectId = static_cast<int>(i);
            }
        }
        return objectId != -1;
    }

    maths::Vector Scene::radiance(const maths::Ray &ray, int depth,
                                  unsigned short *Xi, int emissive) const {
        double t = -1.0;
        int id = -1;
        if (!intersect(ray, t, id))
            return maths::Vector();

        const std::shared_ptr<primitive::IPrimitive> &obj = _primitives.at(id);
        if (depth > kMaxRadianceDepth)
            return maths::Vector();

        maths::Vector rayOrigin(ray.origin.x, ray.origin.y, ray.origin.z);
        maths::Vector x = rayOrigin + ray.direction * t;
        primitive::SurfaceData surfData = obj->surfaceData(x);
        maths::Vector n = (x - obj->center()).normalized();
        maths::Vector nl = n.dot(ray.direction) < 0 ? n : n * -1;

        maths::Vector f = surfData.material.color.toVector();

        double p = f.x > f.y && f.x > f.z ? f.x : f.y > f.z ? f.y : f.z;
        if (++depth > kDiffuseRussianRouletteDepth) {
            if (p <= 0.0)
                return surfData.material.emission * emissive;
            if (::erand48(Xi) < p)
                f = f * (1.0 / p);
            else
                return surfData.material.emission * emissive;
        }

        RadianceContext ctx{x, n, nl, f, depth, Xi, emissive};
        if (surfData.material.reflType == object::primitive::RefltT::DIFF) {
            return radianceDiffuse(ray, *obj, ctx);
        }
        if (surfData.material.reflType == object::primitive::RefltT::SPEC) {
            return radianceSpecular(ray, *obj, ctx);
        }
        return radianceRefractive(ray, *obj, ctx);
    }

    maths::Vector Scene::radianceDiffuse(const maths::Ray &ray,
                                         const primitive::IPrimitive &obj,
                                         const RadianceContext &ctx) const {
        const maths::Vector &x = ctx.x;
        primitive::SurfaceData surfData = obj.surfaceData(x);
        const maths::Vector &n = ctx.n;
        const maths::Vector &nl = ctx.nl;
        const maths::Vector &f = ctx.f;
        int depth = ctx.depth;
        unsigned short *Xi = ctx.Xi;
        int emissive = ctx.emissive;

        double diffuseIntensity = 1.0 - surfData.material.transparency;
        maths::Vector direct(0, 0, 0);

        // Emmissive spheres (NEE)
        for (const auto &p : _primitives) {
            /* TODO:
            if (p->emission().magnitude() > 0) {
                direct += p->computeNEE(scene, x, nl, f);
            }*/
        }

        for (const auto &light : _lights) {
            direct += light->computeNEE(*this, x, nl, f);
        }

        // Ambient light
        maths::Vector ambientContrib(0, 0, 0);
        if (emissive) {
            ambientContrib = _ambientLight.color.toVector() * f;
        }

        // Ambient Occlusion
        maths::Vector aoContrib(0, 0, 0);
        if (_ambiantOcclusion.samples > 0) {
            double unoccluded = 0.0;
            for (int k = 0; k < _ambiantOcclusion.samples; ++k) {
                maths::Vector aoDir = randomCosineDir(nl, Xi);
                maths::Ray aoRay(x + nl * kRayEpsilon, aoDir);
                double aoT;
                int aoId;
                bool hit = intersect(aoRay, aoT, aoId);
                if (!hit || aoT > _ambiantOcclusion.radius)
                    unoccluded += 1.0;
            }
            double aoFactor =
                unoccluded / static_cast<double>(_ambiantOcclusion.samples);
            maths::Vector aoBase =
                _ambientLight.color.toVector().magnitude() > 0
                    ? _ambientLight.color.toVector()
                    : maths::Vector(1, 1, 1);
            aoContrib = aoBase * f * aoFactor;
            ambientContrib += aoContrib;
        }

        // Diffuse contribution (indirect)
        maths::Vector diffuseContrib(0, 0, 0);
        diffuseContrib = _ambientDiffuse.ambient.toVector() * f;

        maths::Vector d = randomCosineDir(nl, Xi);
        return surfData.material.emission * emissive + direct + ambientContrib +
               diffuseContrib * diffuseIntensity +
               f * radiance(maths::Ray(x, d), depth, Xi, 0) * diffuseIntensity;
    }

    maths::Vector Scene::radianceSpecular(const maths::Ray &ray,
                                          const primitive::IPrimitive &obj,
                                          const RadianceContext &ctx) const {
        const maths::Vector &x = ctx.x;
        primitive::SurfaceData surfData = obj.surfaceData(x);
        const maths::Vector &n = ctx.n;
        const maths::Vector &f = ctx.f;
        int depth = ctx.depth;
        unsigned short *Xi = ctx.Xi;
        int emissive = ctx.emissive;

        double smoothness = 1.0 - surfData.material.roughness;
        double metallic = surfData.material.metalness;
        double reflectivity = surfData.material.reflectivity;

        double reflIntensity =
            reflectivity * smoothness * (1.0 + metallic * 2.0);
        maths::Vector reflDir = ray.direction - n * 2 * n.dot(ray.direction);

        return surfData.material.emission * emissive +
               f * radiance(maths::Ray(x, reflDir), depth, Xi, 1) *
                   reflIntensity;
    }

    maths::Vector Scene::radianceRefractive(const maths::Ray &ray,
                                            const primitive::IPrimitive &obj,
                                            const RadianceContext &ctx) const {
        const maths::Vector &x = ctx.x;
        primitive::SurfaceData surfData = obj.surfaceData(x);
        const maths::Vector &n = ctx.n;
        const maths::Vector &f = ctx.f;
        int depth = ctx.depth;
        unsigned short *Xi = ctx.Xi;
        int emissive = ctx.emissive;

        double transparency = surfData.material.transparency;
        double reflectivity =
            surfData.material.reflectivity * (1.0 - transparency);

        maths::Ray reflRay(x, ray.direction - n * 2 * n.dot(ray.direction));
        bool into = n.dot(n * 1) > 0;
        double nc = 1.0;
        double nt =
            surfData.material.ior > 0 ? surfData.material.ior : kDefaultIor;
        double nnt = into ? nc / nt : nt / nc;
        double ddn = ray.direction.dot(n * (n.dot(ray.direction) < 0 ? 1 : -1));
        double cos2t = 1 - nnt * nnt * (1 - ddn * ddn);

        if (cos2t < 0)
            return surfData.material.emission * emissive +
                   f * radiance(reflRay, depth, Xi, 1) * reflectivity;

        maths::Vector tdir =
            (ray.direction * nnt -
             n * ((into ? 1 : -1) * (ddn * nnt + std::sqrt(cos2t))))
                .normalized();

        double a = nt - nc, b = nt + nc;
        double R0 = a * a / (b * b);
        double c = 1 - (into ? -ddn : tdir.dot(n));
        double Re = R0 + (1 - R0) * c * c * c * c * c;
        double Tr = 1 - Re;

        double P = 0.25 + 0.5 * Re * (1.0 - transparency);
        double RP = Re / P, TP = Tr / (1 - P);

        return surfData.material.emission * emissive +
               f * (depth > kRefractiveRussianRouletteDepth
                        ? (::erand48(Xi) < P
                               ? radiance(reflRay, depth, Xi, 1) * RP *
                                     (reflectivity + (1.0 - reflectivity) * Re)
                               : radiance(maths::Ray(x, tdir), depth, Xi, 1) *
                                     TP * transparency)
                        : radiance(reflRay, depth, Xi, 1) * Re *
                                  (reflectivity + (1.0 - reflectivity) * Re) +
                              radiance(maths::Ray(x, tdir), depth, Xi, 1) * Tr *
                                  transparency);
    }
}  // namespace raytracer::object::scene