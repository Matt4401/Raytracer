/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** Noise
*/

#include "math/Noise.hpp"

namespace raytracer::maths {
    const int perm[] = {
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

    double Noise::fade(double t) {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    double Noise::lerp(double a, double b, double t) {
        return a + t * (b - a);
    }

    double Noise::grad(int hash, double x, double y, double z) {
        const int h = hash & 15;
        const double u = h < 8 ? x : y;
        const double v = h < 4 ? y : (h == 12 || h == 14 ? x : z);
        return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
    }

    double Noise::perlin(double x, double y, double z) {
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

    std::pair<double, double> Noise::worley(double x, double y, double z) {
        int cx = static_cast<int>(std::floor(x));
        int cy = static_cast<int>(std::floor(y));
        int cz = static_cast<int>(std::floor(z));

        double f1 = 1e9;
        double f2 = 1e9;

        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                for (int k = -1; k <= 1; ++k) {
                    int cellX = cx + i;
                    int cellY = cy + j;
                    int cellZ = cz + k;

                    int hash =
                        perm[(cellX + perm[(cellY + perm[cellZ & 255]) & 255]) &
                             255];

                    double ptX = (hash & 15) / 15.0;
                    double ptY = ((hash >> 4) & 15) / 15.0;
                    double ptZ = ((hash >> 2) & 15) / 15.0;

                    double dx = (cellX + ptX) - x;
                    double dy = (cellY + ptY) - y;
                    double dz = (cellZ + ptZ) - z;
                    double dist = std::sqrt(dx * dx + dy * dy + dz * dz);

                    if (dist < f1) {
                        f2 = f1;
                        f1 = dist;
                    } else if (dist < f2) {
                        f2 = dist;
                    }
                }
            }
        }
        return {f1, f2};
    }
}  // namespace raytracer::maths