//
// Created by 沛然 姚 on 2017/5/3.
//

#ifndef RAYTRACING_CAMERA_H
#define RAYTRACING_CAMERA_H

#include <opencv2/core/core.hpp>
#include <cmath>

struct Camera {
    cv::Vec3f eye;
    cv::Vec3f u, v, w; // three axes.
    Camera(cv::Vec3f eye) : eye(eye), u(1, 0, 0), v(0, -1, 0), w(0, 0, -1) { }

    void yaw(double deg) {
        auto rad = deg/180*M_PI;
        w = cv::Vec3f((float) (cos(rad) * w[0] - sin(rad) * w[2]),
                      w[1],
                      (float) (sin(rad) * w[0] + cos(rad) * w[2]));
        u = cv::Vec3f((float) (cos(rad) * u[0] - sin(rad) * u[2]),
                      u[1],
                      (float) (sin(rad) * u[0] + cos(rad) * u[2]));
    }

    void pitch(double deg) {
        auto rad = deg/180*M_PI;
        w = cv::Vec3f(w[0],
                      (float) (cos(rad) * w[1] - sin(rad) * w[2]),
                      (float) (sin(rad) * w[1] + cos(rad) * w[2]));
        v = cv::Vec3f(v[0],
                      (float) (cos(rad) * v[1] - sin(rad) * v[2]),
                      (float) (sin(rad) * v[1] + cos(rad) * v[2]));
    }

    void roll(double deg) {
        auto rad = deg/180*M_PI;
        u = cv::Vec3f((float) (cos(rad) * u[0] - sin(rad) * u[1]),
                      (float) (sin(rad) * u[0] + cos(rad) * u[1]),
                      u[2]);
        v = cv::Vec3f((float) (cos(rad) * v[0] - sin(rad) * v[1]),
                      (float) (sin(rad) * v[0] + cos(rad) * v[1]),
                      v[2]);
    }
};

#endif //RAYTRACING_CAMERA_H
