//
// Created by 沛然 姚 on 2017/4/26.
//

#ifndef RAYTRACING_RAY_H
#define RAYTRACING_RAY_H
#include <opencv2/core/core.hpp>

struct Ray {
    cv::Vec3d origin;
    cv::Vec3d direction;

    Ray(cv::Vec3d origin, cv::Vec3d direction) {
        this->origin = origin, this->direction = direction;
    }
};


#endif //RAYTRACING_RAY_H
