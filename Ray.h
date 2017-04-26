//
// Created by 沛然 姚 on 2017/4/26.
//

#ifndef RAYTRACING_RAY_H
#define RAYTRACING_RAY_H
#include <opencv2/core/core.hpp>
#include <cstdio>

struct Ray {
    cv::Vec3d origin;
    cv::Vec3d direction;

    Ray(cv::Vec3d origin, cv::Vec3d direction) {
        this->origin = origin;
        this->direction = direction;
    }

    void repr() const {
        printf("origin: (%f, %f, %f), direction: (%f %f %f)\n", origin[0], origin[1], origin[2], direction[0], direction[1], direction[2]);
    }
};


#endif //RAYTRACING_RAY_H
