/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Scene
*/

#ifndef SCENE_HPP_
#define SCENE_HPP_

#include "object/AScene.hpp"

namespace raytracer::object::scene {

    struct RadianceContext {
        maths::Vector x;
        maths::Vector n;
        maths::Vector nl;
        maths::Vector f;
        primitive::SurfaceData surfData;
        int depth;
        unsigned short *xi;
        const int emissive;
        primitive::HitRecord hitRecord;
    };

    class Scene : public AScene {
      public:
        Scene();
        explicit Scene(const std::map<std::string, std::any> &params);
        ~Scene() override = default;

        bool intersect(const maths::Ray &ray,
                       primitive::HitRecord &record) const override;
        maths::Vector radiance(const maths::Ray &ray, int depth,
                               unsigned short *xi,
                               int emissive = 1) const override;

      private:
        /// @brief build an orthonormal basis (u, v, w) given a normal vector w.
        /// The vectors u and v are perpendicular
        /// to w and to each other. This is used to generate random directions
        /// in the hemisphere around the normal vector for diffuse reflection.
        /// @param w the normal vector, expected to be a non-zero vector. The
        /// function will normalize w internally.
        /// @param u the first vector of the orthonormal basis, perpendicular to
        /// w.
        /// @param v the second vector of the orthonormal basis, perpendicular
        /// to both w and u.
        void buildONB(const maths::Vector &w, maths::Vector &u,
                      maths::Vector &v) const;
        /// @brief generate a random direction in the hemisphere around the
        /// normal vector nl, with a cosine-weighted distribution. This is used
        /// for diffuse reflection to simulate the way light scatters off rough
        /// surfaces, where rays closer to the normal direction are more likely
        /// to be reflected.
        /// @param nl the normal vector defining the hemisphere, expected to be
        /// a non-zero vector. The function will normalize nl internally.
        /// @param xi the random seed state for the drand48 family of functions,
        /// used to generate random numbers. The caller is responsible for
        /// initializing and managing the state of xi.
        /// @return a random direction vector in the hemisphere around nl, with
        /// a cosine-weighted distribution.
        maths::Vector randomCosineDir(const maths::Vector &nl,
                                      unsigned short *xi) const;
        /// @brief Russian Roulette termination test based on weight and depth.
        /// @param depth current ray depth
        /// @param weight the weight factor (color luminance, reflectivity, or
        /// transmission weight)
        /// @param xi random seed state
        /// @return true if the ray should continue tracing, false if terminated
        /// by RR
        bool shouldContinueRussianRoulette(int depth, double weight,
                                           unsigned short *xi) const;
        maths::Vector radianceDiffuse(const RadianceContext &ctx) const;
        maths::Vector radianceSpecular(const maths::Ray &ray,
                                       const RadianceContext &ctx) const;
        maths::Vector radianceRefractive(const maths::Ray &ray,
                                         const RadianceContext &ctx) const;
    };
}  // namespace raytracer::object::scene

#endif /* !SCENE_HPP_ */
