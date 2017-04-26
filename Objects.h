//
// Created by 沛然 姚 on 2017/4/26.
//

#ifndef RAYTRACING_OBJECT_H
#define RAYTRACING_OBJECT_H

#include "Ray.h"
#include <cstdio>
#include <cmath>
#include <algorithm>

class Object {

public:
    static double NO_INTERSECTION;

    cv::Vec3b color;
    cv::Vec3b ks;
    int p;

    virtual ~Object() {};
    virtual double intersect(Ray ray) const = 0;
    virtual void repr() const = 0;
};

class Sphere : public Object {
private:
    cv::Vec3d center;
    double radius;
public:
    Sphere(double x, double y, double z, double radius, cv::Vec3b color = cv::Vec3b(255,255,255), cv::Vec3b ks = cv::Vec3b(255,255,255), int p = 100);
    double intersect(Ray ray) const override;
    void repr() const override;
};


#endif //RAYTRACING_OBJECT_H
