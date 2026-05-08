/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** PerlinNoise
*/

#include "PerlinNoise.hpp"

#include <algorithm>
#include <cmath>

#include "util/middleware/Helpers.hpp"
#include "util/middleware/ObjectMiddleware.hpp"

namespace raytracer::object::material {

    static int perm[] = {
        151, 160, 137, 91,  90,  15,  131, 13,  201, 95,  96,  53,  194, 233,
        7,   225, 140, 36,  103, 30,  69,  142, 8,   99,  37,  240, 21,  10,
        23,  190, 6,   148, 247, 120, 234, 75,  0,   26,  197, 62,  94,  252,
        219, 203, 117, 35,  11,  32,  57,  177, 33,  88,  237, 149, 56,  87,
        174, 20,  125, 136, 171, 168, 68,  175, 74,  165, 71,  134, 139, 48,
        27,  166, 77,  146, 158, 231, 83,  111, 229, 122, 60,  211, 133, 230,
        220, 105, 92,  41,  55,  46,  245, 40,  244, 102, 143, 54,  65,  25,
        63,  161, 1,   216, 80,  73,  209, 76,  132, 187, 208, 89,  18,  169,
        200, 196, 135, 130, 116, 188, 159, 86,  164, 100, 109, 198, 173, 186,
        3,   64,  52,  217, 226, 250, 124, 123, 5,   202, 38,  147, 118, 126,
        255, 82,  85,  212, 207, 206, 59,  227, 47,  16,  58,  17,  182, 189,
        28,  42,  223, 183, 170, 213, 119, 248, 152, 2,   44,  154, 163, 70,
        221, 153, 101, 155, 167, 43,  172, 9,   129, 22,  39,  253, 19,  98,
        108, 110, 79,  113, 224, 232, 178, 185, 112, 104, 218, 246, 97,  228,
        251, 34,  242, 193, 238, 210, 144, 12,  191, 179, 162, 241, 81,  51,
        145, 235, 249, 14,  239, 107, 49,  192, 214, 31,  181, 199, 106, 157,
        184, 84,  204, 176, 115, 121, 50,  45,  127, 4,   150, 254, 138, 236,
        205, 93,  222, 114, 67,  29,  24,  72,  243, 141, 128, 195, 78,  66,
        215, 61,  156, 180, 151, 160, 137, 91,  90,  15,  131, 13,  201, 95,
        96,  53,  194, 233, 7,   225, 140, 36,  103, 30,  69,  142, 8,   99,
        37,  240, 21,  10,  23,  190, 6,   148, 247, 120, 234, 75,  0,   26,
        197, 62,  94,  252, 219, 203, 117, 35,  11,  32,  57,  177, 33,  88,
        237, 149, 56,  87,  174, 20,  125, 136, 171, 168, 68,  175, 74,  165,
        71,  134, 139, 48,  27,  166, 77,  146, 158, 231, 83,  111, 229, 122,
        60,  211, 133, 230, 220, 105, 92,  41,  55,  46,  245, 40,  244, 102,
        143, 54,  65,  25,  63,  161, 1,   216, 80,  73,  209, 76,  132, 187,
        208, 89,  18,  169, 200, 196, 135, 130, 116, 188, 159, 86,  164, 100,
        109, 198, 173, 186, 3,   64,  52,  217, 226, 250, 124, 123, 5,   202,
        38,  147, 118, 126, 255, 82,  85,  212, 207, 206, 59,  227, 47,  16,
        58,  17,  182, 189, 28,  42,  223, 183, 170, 213, 119, 248, 152, 2,
        44,  154, 163, 70,  221, 153, 101, 155, 167, 43,  172, 9,   129, 22,
        39,  253, 19,  98,  108, 110, 79,  113, 224, 232, 178, 185, 112, 104,
        218, 246, 97,  228, 251, 34,  242, 193, 238, 210, 144, 12,  191, 179,
        162, 241, 81,  51,  145, 235, 249, 14,  239, 107, 49,  192, 214, 31,
        181, 199, 106, 157, 184, 84,  204, 176, 115, 121, 50,  45,  127, 4,
        150, 254, 138, 236, 205, 93,  222, 114, 67,  29,  24,  72,  243, 141,
        128, 195, 78,  66,  215, 61,  156, 180};

    static inline double fade(double t) {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    static inline double lerp(double a, double b, double t) {
        return a + t * (b - a);
    }

    static inline double grad(int hash, double x, double y, double z) {
        const int h = hash & 15;
        const double u = h < 8 ? x : y;
        const double v = h < 4 ? y : (h == 12 || h == 14 ? x : z);
        return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
    }

    static double perlin(double x, double y, double z) {
        const int floorX = static_cast<int>(std::floor(x)) & 255;
        const int floorY = static_cast<int>(std::floor(y)) & 255;
        const int floorZ = static_cast<int>(std::floor(z)) & 255;

        x -= std::floor(x);
        y -= std::floor(y);
        z -= std::floor(z);

        const double u = fade(x);
        const double v = fade(y);
        const double w = fade(z);

        const int a = perm[floorX] + floorY;
        const int aa = perm[a] + floorZ;
        const int ab = perm[a + 1] + floorZ;
        const int b = perm[floorX + 1] + floorY;
        const int ba = perm[b] + floorZ;
        const int bb = perm[b + 1] + floorZ;

        const double res = lerp(
            lerp(lerp(grad(perm[aa], x, y, z), grad(perm[ba], x - 1, y, z), u),
                 lerp(grad(perm[ab], x, y - 1, z),
                      grad(perm[bb], x - 1, y - 1, z), u),
                 v),
            lerp(lerp(grad(perm[aa + 1], x, y, z - 1),
                      grad(perm[ba + 1], x - 1, y, z - 1), u),
                 lerp(grad(perm[ab + 1], x, y - 1, z - 1),
                      grad(perm[bb + 1], x - 1, y - 1, z - 1), u),
                 v),
            w);
        return res;
    }

    PerlinNoise::PerlinNoise(const std::map<std::string, std::any>& args)
        : ABasicMaterial(args),
          _color1(util::Helpers::toColor(args, "color1", "PerlinNoise")),
          _color2(util::Helpers::toColor(args, "color2", "PerlinNoise")),
          _scale(1.0),
          _octaves(1),
          _persistence(0.5) {
        _scale = util::ObjectMiddleware::optional<double>(args, "scale", 1.0,
                                                          "PerlinNoise");
        util::Helpers::unsignedDouble(_scale, "scale", "PerlinNoise");
        if (_scale <= 0.0)
            _scale = 1.0;

        _octaves = util::ObjectMiddleware::optional<int>(args, "octaves", 1,
                                                         "PerlinNoise");
        if (_octaves < 1)
            _octaves = 1;

        _persistence = util::ObjectMiddleware::optional<double>(
            args, "persistence", 0.5, "PerlinNoise");
        util::Helpers::unsignedDouble(_persistence, "persistence",
                                      "PerlinNoise");
        if (_persistence <= 0.0)
            _persistence = 0.5;
    }

    primitive::MaterialProperties PerlinNoise::evaluate(
        const primitive::SurfaceData& data,
        const maths::Vector& hitPoint) const {
        const double x = hitPoint.x * _scale;
        const double y = hitPoint.y * _scale;
        const double z = hitPoint.z * _scale;

        double amplitude = 1.0;
        double frequency = 1.0;
        double noise = 0.0;
        double maxValue = 0.0;
        for (int i = 0; i < _octaves; ++i) {
            noise +=
                perlin(x * frequency, y * frequency, z * frequency) * amplitude;
            maxValue += amplitude;
            amplitude *= _persistence;
            frequency *= 2.0;
        }
        noise /= maxValue;

        const double n = (noise + 1.0) * 0.5;
        maths::Color finalColor = maths::Color(
            static_cast<unsigned char>(std::clamp(
                (int)std::round(_color1.r * (1 - n) + _color2.r * n), 0, 255)),
            static_cast<unsigned char>(std::clamp(
                (int)std::round(_color1.g * (1 - n) + _color2.g * n), 0, 255)),
            static_cast<unsigned char>(std::clamp(
                (int)std::round(_color1.b * (1 - n) + _color2.b * n), 0, 255)));

        return {.color = finalColor,
                .emission = _emission,
                .reflType = _refl,
                .reflectivity = _reflectivity,
                .transparency = _transparency,
                .ior = _ior,
                .roughness = _roughness,
                .metalness = _metalness};
    }

}  // namespace raytracer::object::material
