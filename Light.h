//
// Created by 沛然 姚 on 2017/4/26.
//

#ifndef RAYTRACING_LIGHT_H
#define RAYTRACING_LIGHT_H

#include <opencv2/core/core.hpp>
#include <cstdio>

/*
 * point light source
 */
struct Light {
    cv::Vec3d position;
    double intensity;

    Light(cv::Vec3d pos, double intensity = 1) {
        this->position = pos;
        this->intensity = intensity;
    }

    void repr() const {
        printf("Pos: (%.2f %.2f %.2f), intensity %.2f\n", position[0], position[1], position[2], intensity);
    }
};


#endif //RAYTRACING_LIGHT_H
