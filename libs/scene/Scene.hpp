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
        int depth;
        unsigned short *Xi;
        const int emissive;
    };

    class Scene : public AScene {
      public:
        Scene();
        explicit Scene(const std::map<std::string, std::any> &params);
        ~Scene() override = default;

        bool intersect(const maths::Ray &ray, double &t,
                       int &objectId) const override;
        maths::Vector radiance(const maths::Ray &ray, int depth,
                               unsigned short *Xi, const int emissive = 1) override;

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
        static void buildONB(const maths::Vector &w, maths::Vector &u,
                      maths::Vector &v);
        /// @brief generate a random direction in the hemisphere around the
        /// normal vector nl, with a cosine-weighted distribution. This is used
        /// for diffuse reflection to simulate the way light scatters off rough
        /// surfaces, where rays closer to the normal direction are more likely
        /// to be reflected.
        /// @param nl the normal vector defining the hemisphere, expected to be
        /// a non-zero vector. The function will normalize nl internally.
        /// @param Xi the random seed state for the drand48 family of functions,
        /// used to generate random numbers. The caller is responsible for
        /// initializing and managing the state of Xi.
        /// @return a random direction vector in the hemisphere around nl, with
        /// a cosine-weighted distribution.
        maths::Vector randomCosineDir(const maths::Vector &nl,
                                      unsigned short *Xi);
        maths::Vector radianceDiffuse(const maths::Ray &ray,
                                      const primitive::IPrimitive &obj,
                                      const RadianceContext &ctx);
        maths::Vector radianceSpecular(const maths::Ray &ray,
                                       const primitive::IPrimitive &obj,
                                       const RadianceContext &ctx);
        maths::Vector radianceRefractive(const maths::Ray &ray,
                                         const primitive::IPrimitive &obj,
                                         const RadianceContext &ctx);
    };
}  // namespace raytracer::object::scene

#endif /* !SCENE_HPP_ */
