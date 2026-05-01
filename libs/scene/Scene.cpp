/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Scene
*/

#include "Scene.hpp"

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
                                  unsigned short *Xi, int emissive) {
        double t = -1.0;
        int id = -1;
        if (!intersect(ray, t, id))
            return maths::Vector();

        const std::unique_ptr<primitive::IPrimitive> &obj = _primitives.at(id);
        if (depth > 10)
            return maths::Vector();

        maths::Vector rayOrigin(ray.origin.x, ray.origin.y, ray.origin.z);
        maths::Vector x = rayOrigin + ray.direction * t;
        primitive::SurfaceData surfData = obj->surfaceData(x);
        maths::Vector n = (x - obj->center()).normalized();
        maths::Vector nl = n.dot(ray.direction) < 0 ? n : n * -1;

        maths::Vector f = surfData.color.toVector() * (1.0 / 255.0);

        double p = f.x > f.y && f.x > f.z ? f.x : f.y > f.z ? f.y : f.z;
        if (++depth > 5) {
            if (p <= 0.0)
                return surfData.emission * emissive;
            if (::erand48(Xi) < p)
                f = f * (1.0 / p);
            else
                return surfData.emission * emissive;
        }

        RadianceContext ctx{x, n, nl, f, depth, Xi, emissive};
        if (surfData.reflType == primitive::RefltT::DIFF) {
            return radianceDiffuse(ray, *obj, ctx);
        }
        if (surfData.reflType == primitive::RefltT::SPEC) {
            return radianceSpecular(ray, *obj, ctx);
        }
        return radianceRefractive(ray, *obj, ctx);
    }

    void Scene::buildONB(const maths::Vector &w, maths::Vector &u,
                         maths::Vector &v) {
        double ux = std::fabs(w.x) > 0.1 ? 0 : 1;
        double uy = std::fabs(w.y) > 0.1 ? 0 : 1;
        double uz = std::fabs(w.z) > 0.1 ? 0 : 1;
        u = maths::Vector(ux, uy, uz).cross(w).normalized();
        v = w.cross(u);
    }

    maths::Vector Scene::randomCosineDir(const maths::Vector &nl,
                                         unsigned short *Xi) {
        double r1 = 2 * M_PI * ::erand48(Xi);
        double r2 = ::erand48(Xi);
        double r2s = std::sqrt(r2);
        maths::Vector u, v;
        buildONB(nl, u, v);
        return (u * std::cos(r1) * r2s + v * std::sin(r1) * r2s +
                nl * std::sqrt(1 - r2))
            .normalized();
    }

    maths::Vector Scene::radianceDiffuse(const maths::Ray &ray,
                                         const primitive::IPrimitive &obj,
                                         const RadianceContext &ctx) {
        const maths::Vector &x = ctx.x;
        const maths::Vector &n = ctx.n;
        const maths::Vector &nl = ctx.nl;
        const maths::Vector &f = ctx.f;
        primitive::SurfaceData surfData = obj.surfaceData(x);
        int depth = ctx.depth;
        unsigned short *Xi = ctx.Xi;
        int emissive = ctx.emissive;
        maths::Vector direct(0, 0, 0);

        /* TODO: Emmissive spheres (NEE)
        for (const auto &emmisiveObject : _primitives) {
            // TODO: direct += emmisiveObject->computeNEE(*this, x, nl, f);
        }*/

        // Lights (NEE)
        for (const auto &light : _lights) {
            direct += light->computeNEE(*this, x, nl, f);
        }

        // Ambient light
        maths::Vector ambientContrib(0, 0, 0);
        if (emissive) {
            ambientContrib = _ambientDiffuse.ambient.toVector() * f;
        }

        // Ambient Occlusion
        maths::Vector aoContrib(0, 0, 0);
        if (_ambiantOcclusion.samples > 0) {
            double unoccluded = 0.0;
            for (int k = 0; k < _ambiantOcclusion.samples; ++k) {
                maths::Vector aoDir = randomCosineDir(nl, Xi);
                maths::Ray aoRay(x + nl * 1e-4, aoDir);
                double aoT;
                int aoId;
                bool hit = intersect(aoRay, aoT, aoId);
                if (!hit || aoT > _ambiantOcclusion.radius)
                    unoccluded += 1.0;
            }
            double aoFactor =
                unoccluded / static_cast<double>(_ambiantOcclusion.samples);
            maths::Vector aoBase =
                _ambientDiffuse.ambient.toVector().magnitude() > 0
                    ? _ambientDiffuse.ambient.toVector()
                    : maths::Vector(1, 1, 1);
            aoContrib = aoBase * f * aoFactor;
            ambientContrib += aoContrib;
        }

        // Diffuse contribution (indirect)
        maths::Vector diffuseContrib(0, 0, 0);
        diffuseContrib = _ambientDiffuse.ambient.toVector() * f;

        maths::Vector d = randomCosineDir(nl, Xi);
        return surfData.emission * emissive + direct + ambientContrib +
               diffuseContrib + f * radiance(maths::Ray(x, d), depth, Xi, 0);
    }

    maths::Vector Scene::radianceSpecular(const maths::Ray &ray,
                                          const primitive::IPrimitive &obj,
                                          const RadianceContext &ctx) {
        const maths::Vector &x = ctx.x;
        const maths::Vector &n = ctx.n;
        const maths::Vector &f = ctx.f;
        primitive::SurfaceData surfData = obj.surfaceData(x);

        int depth = ctx.depth;
        unsigned short *Xi = ctx.Xi;
        int emissive = ctx.emissive;
        maths::Vector hitpoint = x + n * 1e-4;
        return surfData.emission * emissive +
               f * radiance(maths::Ray(x, ray.direction -
                                              n * 2 * n.dot(ray.direction)),
                            depth, Xi, 1);
    }

    maths::Vector Scene::radianceRefractive(const maths::Ray &ray,
                                            const primitive::IPrimitive &obj,
                                            const RadianceContext &ctx) {
        const maths::Vector &x = ctx.x;
        const maths::Vector &n = ctx.n;
        const maths::Vector &f = ctx.f;
        primitive::SurfaceData surfData = obj.surfaceData(x);

        int depth = ctx.depth;
        unsigned short *Xi = ctx.Xi;
        int emissive = ctx.emissive;
        maths::Ray reflRay(x, ray.direction - n * 2 * n.dot(ray.direction));
        bool into = n.dot(n * 1) > 0;
        double nc = 1.0;
        double nt = surfData.extraParams.count("ior") > 0
                        ? surfData.extraParams.at("ior")
                        : 1.5;
        double nnt = into ? nc / nt : nt / nc;
        double ddn = ray.direction.dot(n * (n.dot(ray.direction) < 0 ? 1 : -1));
        double cos2t = 1 - nnt * nnt * (1 - ddn * ddn);

        if (cos2t < 0) {
            return surfData.emission * emissive +
                   f * radiance(reflRay, depth, Xi, 1);
        }

        maths::Vector tdir =
            (ray.direction * nnt -
             n * ((into ? 1 : -1) * (ddn * nnt + std::sqrt(cos2t))))
                .normalized();

        double a = nt - nc, b = nt + nc;
        double R0 = a * a / (b * b);
        double c = 1 - (into ? -ddn : tdir.dot(n));
        double Re = R0 + (1 - R0) * c * c * c * c * c;
        double Tr = 1 - Re;
        double P = 0.25 + 0.5 * Re;
        double RP = Re / P, TP = Tr / (1 - P);

        if (depth > 2) {
            if (::erand48(Xi) < P)
                return surfData.emission * emissive +
                       f * radiance(reflRay, depth, Xi, 1) * RP;
            else
                return surfData.emission * emissive +
                       f * radiance(maths::Ray(x, tdir), depth, Xi, 1) * TP;
        } else {
            return surfData.emission * emissive +
                   f * (radiance(reflRay, depth, Xi, 1) * Re +
                        radiance(maths::Ray(x, tdir), depth, Xi, 1) * Tr);
        }
    }
}  // namespace raytracer::object::scene