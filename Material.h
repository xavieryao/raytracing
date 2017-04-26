//
// Created by 沛然 姚 on 2017/4/26.
//

#ifndef RAYTRACING_MATERIAL_H
#define RAYTRACING_MATERIAL_H

#include "Ray.h"

struct Material {
    cv::Vec3b color; /* kd: diffuse coefficient, or the surface color */
    cv::Vec3b ks; /* ks: specular coefficient, or the specular color, of the surface. */
    cv::Vec3b ka; /* ka: the surface's ambient coefficient, or the ambient color. */
    int p; /* Phong exponent */

    Material() : ks(cv::Vec3b(255, 255, 255)), p(100) {}

    Material(cv::Vec3b color, cv::Vec3b ks, cv::Vec3b ka, int p) {
        this->color = color;
        this->ks = ks;
        this->ka = ka;
        this->p = p;
    }
};


#endif //RAYTRACING_MATERIAL_H
