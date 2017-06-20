//
// Created by 沛然 姚 on 2017/5/3.
//

#ifndef RAYTRACING_CAMERA_H
#define RAYTRACING_CAMERA_H

#include <opencv2/core/core.hpp>
#include <cmath>

struct Camera {
    cv::Vec3d eye;
    cv::Vec3d u, v, w; // three axes.
    Camera(cv::Vec3d eye) : eye(eye), u(1, 0, 0), v(0, -1, 0), w(0, 0, -1) { }

    void yaw(double deg) {
        auto rad = deg/180*M_PI;
        w = cv::Vec3d(cos(rad) * w[0] - sin(rad) * w[2],
                      w[1],
                      sin(rad) * w[0] + cos(rad) * w[2]);
        u = cv::Vec3d(cos(rad) * u[0] - sin(rad) * u[2],
                      u[1],
                      sin(rad) * u[0] + cos(rad) * u[2]);
    }

    void pitch(double deg) {
        auto rad = deg/180*M_PI;
        w = cv::Vec3d(w[0],
                      cos(rad) * w[1] - sin(rad) * w[2],
                      sin(rad) * w[1] + cos(rad) * w[2]);
        v = cv::Vec3d(v[0],
                      cos(rad) * v[1] - sin(rad) * v[2],
                      sin(rad) * v[1] + cos(rad) * v[2]);
    }

    void roll(double deg) {
        auto rad = deg/180*M_PI;
        u = cv::Vec3d(cos(rad) * u[0] - sin(rad) * u[1],
                      sin(rad) * u[0] + cos(rad) * u[1],
                      u[2]);
        v = cv::Vec3d(cos(rad) * v[0] - sin(rad) * v[1],
                      sin(rad) * v[0] + cos(rad) * v[1],
                      v[2]);
    }

    static double random() {
        constexpr double HALF_RANGE = 0.005;
        return -HALF_RANGE + static_cast<double>(rand()) / (RAND_MAX/(HALF_RANGE*2));
    };

    cv::Vec3d randomEye() {
        return eye + random()*u + random()*v;
    }
};

#endif //RAYTRACING_CAMERA_H
