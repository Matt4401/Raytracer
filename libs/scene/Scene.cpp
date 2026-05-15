/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Scene
*/

#include "Scene.hpp"

#include <algorithm>
#include <bvh/BVHBuilder.hpp>
#include <limits>
#include <numeric>

#include "object/IScene.hpp"
#include "object/primitive/IPrimitive.hpp"
#include "object/primitive/ReflTypes.hpp"

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
                                         unsigned short *xi) const {
        double r1 = 2 * M_PI * ::erand48(xi);
        double r2 = ::erand48(xi);
        double r2s = std::sqrt(r2);
        maths::Vector u, v;
        buildONB(nl, u, v);
        return (u * std::cos(r1) * r2s + v * std::sin(r1) * r2s +
                nl * std::sqrt(1 - r2))
            .normalized();
    }

    bool Scene::shouldContinueRussianRoulette(int depth, double weight,
                                              unsigned short *xi) const {
        if (depth <= K_DIFFUSE_RUSSIAN_ROULETTE_DEPTH)
            return true;
        if (weight <= 0.0)
            return false;
        return ::erand48(xi) < weight;
    }

    bool Scene::intersect(const maths::Ray &ray,
                          primitive::HitRecord &record) const {
        constexpr double INF = std::numeric_limits<double>::infinity();
        record.t = INF;
        record.objectId = -1;

        if (_bvhRoot) {
            if (_bvhRoot->hits(ray, record)) {
                return true;
            }
            return false;
        } else {
            bool hitAnything = false;
            primitive::HitRecord closestRecord;
            double closestT = INF;

            for (const auto &primitive : _primitives) {
                if (!primitive)
                    continue;
                primitive::HitRecord tempRecord;
                if (primitive->hits(ray, tempRecord) &&
                    tempRecord.t < closestT) {
                    closestT = tempRecord.t;
                    closestRecord = tempRecord;
                    hitAnything = true;
                }
            }

            if (hitAnything) {
                record = closestRecord;
                return true;
            }
            return false;
        }
    }

    maths::Vector Scene::radiance(const maths::Ray &ray, int depth,
                                  unsigned short *xi, int emissive) const {
        if (depth > K_MAX_RADIANCE_DEPTH)
            return maths::Vector();
        primitive::HitRecord hitRecord;
        if (!intersect(ray, hitRecord))
            return maths::Vector();

        hitRecord.hitPoint = ray.origin + ray.direction * hitRecord.t;
        const std::shared_ptr<primitive::IPrimitive> &obj =
            _primitives.at(hitRecord.objectId);

        primitive::SurfaceData surfData = obj->surfaceData(hitRecord);
        maths::Vector x = hitRecord.hitPoint;
        maths::Vector n = surfData.normal;
        maths::Vector nl = n.dot(ray.direction) < 0 ? n : n * -1;

        maths::Vector f = surfData.material.color.toVector();

        ++depth;

        RadianceContext ctx{x,     n,  nl,       f,        surfData,
                            depth, xi, emissive, hitRecord};
        if (surfData.material.reflType == object::primitive::RefltT::DIFF) {
            return radianceDiffuse(ctx);
        }
        if (surfData.material.reflType == object::primitive::RefltT::SPEC) {
            return radianceSpecular(ray, ctx);
        }
        return radianceRefractive(ray, ctx);
    }

    maths::Vector Scene::radianceDiffuse(const RadianceContext &ctx) const {
        const maths::Vector &x = ctx.x;
        const maths::Vector &nl = ctx.nl;
        const maths::Vector &f = ctx.f;
        int depth = ctx.depth;
        unsigned short *xi = ctx.xi;
        int emissive = ctx.emissive;
        const primitive::SurfaceData &surfData = ctx.surfData;
        const maths::Vector emitted = surfData.material.emission * emissive;
        const maths::Vector ambientLightColor = _ambientLight.color.toVector();
        const maths::Vector ambientDiffuseColor =
            _ambientDiffuse.ambient.toVector();
        const double ambientLightScale = _ambientLight.intensity / M_PI;
        const double ambientDiffuseScale = _ambientDiffuse.intensity / M_PI;

        double p = f.x > f.y && f.x > f.z ? f.x : f.y > f.z ? f.y : f.z;
        if (!shouldContinueRussianRoulette(depth, p, xi))
            return emitted;

        const double roughness =
            std::clamp(surfData.material.roughness, 0.0, 1.0);
        const maths::Vector diffuseF = f;

        maths::Vector direct(0, 0, 0);
        for (const auto &light : _lights) {
            direct += light->computeNEE(*this, x, nl, diffuseF);
        }

        // Ambient light
        maths::Vector ambientContrib(0, 0, 0);
        if (emissive) {
            ambientContrib = ambientLightColor * ambientLightScale * f;
        }

        // Ambient Occlusion
        maths::Vector aoContrib(0, 0, 0);
        if (_ambientOcclusion.samples > 0) {
            double unoccluded = 0.0;
            for (int k = 0; k < _ambientOcclusion.samples; ++k) {
                maths::Vector aoDir = randomCosineDir(nl, xi);
                maths::Ray aoRay(x + nl * K_RAY_EPSILON, aoDir);
                primitive::HitRecord aoRecord;
                bool hit = intersect(aoRay, aoRecord);
                if (!hit || aoRecord.t > _ambientOcclusion.radius)
                    unoccluded += 1.0;
            }
            double aoFactor =
                unoccluded / static_cast<double>(_ambientOcclusion.samples);
            maths::Vector aoBase =
                ambientLightColor.magnitude() > 0
                    ? ambientLightColor * ambientLightScale
                    : maths::Vector(1, 1, 1) * ambientLightScale;
            aoContrib = aoBase * diffuseF * aoFactor;
            ambientContrib += aoContrib;
        }

        // Ambient diffuse contribution
        maths::Vector diffuseContrib(0, 0, 0);
        diffuseContrib = ambientDiffuseColor * ambientDiffuseScale * diffuseF;

        // Diffuse indirect
        maths::Vector diffuseDir = randomCosineDir(nl, xi);
        if (roughness > 0.0) {
            diffuseDir = (diffuseDir * (1.0 - roughness) +
                          randomCosineDir(nl, xi) * roughness)
                             .normalized();
        }

        return emitted + direct + ambientContrib + diffuseContrib +
               diffuseF * radiance(maths::Ray(x, diffuseDir), depth, xi, 0);
    }

    maths::Vector Scene::radianceSpecular(const maths::Ray &ray,
                                          const RadianceContext &ctx) const {
        const maths::Vector &x = ctx.x;
        const maths::Vector &n = ctx.n;
        const maths::Vector &nl = ctx.nl;
        const maths::Vector &f = ctx.f;
        const primitive::SurfaceData &surfData = ctx.surfData;
        int depth = ctx.depth;
        unsigned short *xi = ctx.xi;
        int emissive = ctx.emissive;
        const double reflectivity =
            std::clamp(surfData.material.reflectivity, 0.0, 1.0);

        if (!shouldContinueRussianRoulette(depth, reflectivity, xi))
            return surfData.material.emission * emissive;

        const double roughness =
            std::clamp(surfData.material.roughness, 0.0, 1.0);

        maths::Vector reflectionDir =
            ray.direction - n * 2 * n.dot(ray.direction);

        if (roughness > 0.0) {
            maths::Vector u, v;
            buildONB(reflectionDir, u, v);
            double r1 = 2 * M_PI * ::erand48(xi);
            double r2 = ::erand48(xi) * roughness;
            double r2s = std::sqrt(r2);
            maths::Vector perturb = u * std::cos(r1) * r2s +
                                    v * std::sin(r1) * r2s +
                                    reflectionDir * std::sqrt(1.0 - r2);
            reflectionDir = perturb.normalized();
        }

        const maths::Vector specularWeight = f * reflectivity;

        return surfData.material.emission * emissive +
               specularWeight *
                   radiance(maths::Ray(x + nl * K_RAY_EPSILON, reflectionDir),
                            depth, xi, 1);
    }

    maths::Vector Scene::radianceRefractive(const maths::Ray &ray,
                                            const RadianceContext &ctx) const {
        const maths::Vector &x = ctx.x;
        const maths::Vector &n = ctx.n;
        const maths::Vector &nl = ctx.nl;
        const maths::Vector &f = ctx.f;
        int depth = ctx.depth;
        unsigned short *xi = ctx.xi;
        int emissive = ctx.emissive;
        const primitive::SurfaceData &surfData = ctx.surfData;

        const double ior =
            surfData.material.ior > 0 ? surfData.material.ior : K_DEFAULT_IOR;
        const double roughness =
            std::clamp(surfData.material.roughness, 0.0, 1.0);
        const double transparency =
            std::clamp(surfData.material.transparency, 0.0, 1.0);

        maths::Ray reflRay(x, ray.direction - n * 2 * n.dot(ray.direction));

        bool into = n.dot(nl) > 0;
        double nc = 1.0;
        double nt = ior;
        double nnt = into ? nc / nt : nt / nc;
        double ddn = ray.direction.dot(n * (n.dot(ray.direction) < 0 ? 1 : -1));
        double cos2t = 1 - nnt * nnt * (1 - ddn * ddn);

        if (cos2t < 0) {
            return surfData.material.emission * emissive +
                   f * radiance(reflRay, depth, xi, 1);
        }

        maths::Vector tdir =
            (ray.direction * nnt -
             n * ((into ? 1 : -1) * (ddn * nnt + std::sqrt(cos2t))))
                .normalized();

        if (roughness > 0.0) {
            tdir =
                (tdir * (1.0 - roughness) + randomCosineDir(nl, xi) * roughness)
                    .normalized();
        }

        double nDelta = nt - nc, nSum = nt + nc;
        double reflectance0 = nDelta * nDelta / (nSum * nSum);
        double angleFactor = 1 - (into ? -ddn : tdir.dot(n));
        double reflectance = reflectance0 + (1 - reflectance0) * angleFactor *
                                                angleFactor * angleFactor *
                                                angleFactor * angleFactor;
        double transmittance = 1 - reflectance;

        transmittance *= transparency;

        double totalWeight = reflectance + transmittance;

        if (!shouldContinueRussianRoulette(depth, totalWeight, xi))
            return surfData.material.emission * emissive;

        double sum = reflectance + transmittance;
        double reflWeight = 0.0;
        double transWeight = 0.0;
        if (sum > K_PROB_NORMALIZATION_THRESHOLD) {
            reflWeight = reflectance / sum;
            transWeight = transmittance / sum;
        } else {
            reflWeight = 0.5;
            transWeight = 0.5;
        }

        double choiceProb = 0.25 + 0.5 * reflWeight;
        double reflectProb = reflWeight / choiceProb;
        double transProb = transWeight / (1 - choiceProb);

        if (depth > K_REFRACTIVE_RUSSIAN_ROULETTE_DEPTH) {
            if (::erand48(xi) < choiceProb)
                return surfData.material.emission * emissive +
                       f * radiance(reflRay, depth, xi, 1) * reflectProb;
            else
                return surfData.material.emission * emissive +
                       f * radiance(maths::Ray(x, tdir), depth, xi, 1) *
                           transProb;
        } else {
            return surfData.material.emission * emissive +
                   f * (radiance(reflRay, depth, xi, 1) * reflectance +
                        radiance(maths::Ray(x, tdir), depth, xi, 1) *
                            transmittance);
        }
    }
}  // namespace raytracer::object::scene
