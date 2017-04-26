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

    cv::Vec3b color; /* kd: diffuse coefficient, or the surface color */
    cv::Vec3b ks; /* ks: specular coefficient, or the specular color, of the surface. */
    cv::Vec3b ka; /* ka: the surface's ambient coefficient, or the ambient color. */
    int p; /* Phong exponent */

    virtual ~Object() {};
    virtual double intersect(Ray ray) const = 0;
    virtual void repr() const = 0;
    virtual cv::Vec3d normalVector(cv::Vec3d point) const = 0;
};

class Sphere : public Object {
private:
    cv::Vec3d center;
    double radius;
public:
    Sphere(cv::Vec3d center, double radius
            , cv::Vec3b color = cv::Vec3b(255,255,255)
            , cv::Vec3b ks = cv::Vec3b(255,255,255)
            , cv::Vec3b ka = cv::Vec3b(255, 255, 255)
            , int p = 100);
    double intersect(Ray ray) const override;
    void repr() const override;
    cv::Vec3d normalVector(cv::Vec3d point) const override;
};


#endif //RAYTRACING_OBJECT_H
