//
// Created by 沛然 姚 on 2017/4/26.
//

#ifndef RAYTRACING_MATERIAL_H
#define RAYTRACING_MATERIAL_H

#include "Ray.h"
#include <cstdlib>

struct Material {
    cv::Vec3b color; /* kd: diffuse coefficient, or the surface color */
    cv::Vec3b ks; /* ks: specular coefficient, or the specular color, of the surface. */
    cv::Vec3b ka; /* ka: the surface's ambient coefficient, or the ambient color. */
    float km; /* km: mirror coefficient */
    int p; /* Phong exponent */
    bool dielectric;
    float ar, ag, ab; /* RGB refraction factor */

    Material() : ks(cv::Vec3b(255, 255, 255)), p(100) {}

    Material(cv::Vec3b color, cv::Vec3b ks, cv::Vec3b ka, float km, int p) {
        this->color = color;
        this->ks = ks;
        this->ka = ka;
        this->p = p;
        this->km = km;
        this->dielectric = false;
    }

    static Material randomMaterial() {
        cv::Vec3b color, ks, ka;
        for (int i = 0; i < 3; ++i) {
            color[i] = static_cast<unsigned char>(rand()%256);
        }
        for (int i = 0; i < 3; ++i) {
            ks[i] = static_cast<unsigned char>(rand()%256);
        }
        for (int i = 0; i < 3; ++i) {
            ka[i] = static_cast<unsigned char>(rand()%256);
        }
        Material m(color, ks, ka, 0, rand()%150);
        return m;
    }
};


#endif //RAYTRACING_MATERIAL_H
