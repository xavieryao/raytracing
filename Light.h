//
// Created by 沛然 姚 on 2017/4/26.
//

#ifndef RAYTRACING_LIGHT_H
#define RAYTRACING_LIGHT_H

#include <opencv2/core/core.hpp>
#include <cstdio>
#include <cstdlib>

/*
 * point light source
 */
struct Light {
    cv::Vec3d corner;
    cv::Vec3d edgeB;
    cv::Vec3d edgeA;
    double intensity;

    Light(cv::Vec3d corner, cv::Vec3d edgeA, cv::Vec3d edgeB, double intensity = 1) {
        this->corner = corner;
        this->edgeA = edgeA;
        this->edgeB = edgeB;
        this->intensity = intensity;
    }

    cv::Vec3d randomPoint() {
        return this->corner + random()*this->edgeA + random()*this->edgeB;
    }

    static double random() {
        return static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
    };

};


#endif //RAYTRACING_LIGHT_H
