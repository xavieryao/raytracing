//
// Created by 沛然 姚 on 2017/4/26.
//

#ifndef RAYTRACING_OBJECT_H
#define RAYTRACING_OBJECT_H

#include "Ray.h"
#include <cstdio>
#include <cmath>
#include <algorithm>

/*
 * axis aligned bounding box
 */
struct AABB {
    cv::Vec3f bounds[2];

    AABB(cv::Vec3f min, cv::Vec3f max) {
        this->bounds[0] = min;
        this->bounds[1] = max;
    }

    bool intersect(const Ray &r) const
    {
        float tmin, tmax, tymin, tymax, tzmin, tzmax;

        tmin = (bounds[r.sign[0]][0] - r.origin[0]) * r.invDir[0];
        tmax = (bounds[1-r.sign[0]][0] - r.origin[0]) * r.invDir[0];
        tymin = (bounds[r.sign[1]][1] - r.origin[1]) * r.invDir[1];
        tymax = (bounds[1-r.sign[1]][1] - r.origin[1]) * r.invDir[1];

        if ((tmin > tymax) || (tymin > tmax))
            return false;
        if (tymin > tmin)
            tmin = tymin;
        if (tymax < tmax)
            tmax = tymax;

        tzmin = (bounds[r.sign[2]][2] - r.origin[2]) * r.invDir[2];
        tzmax = (bounds[1-r.sign[2]][2] - r.origin[2]) * r.invDir[2];

        if ((tmin > tzmax) || (tzmin > tmax))
            return false;
        if (tzmin > tmin)
            tmin = tzmin;
        if (tzmax < tmax)
            tmax = tzmax;

        return true;
    }
};

class Object {

public:
    static float NO_INTERSECTION;

    cv::Vec3b color; /* kd: diffuse coefficient, or the surface color */
    cv::Vec3b ks; /* ks: specular coefficient, or the specular color, of the surface. */
    cv::Vec3b ka; /* ka: the surface's ambient coefficient, or the ambient color. */
    int p; /* Phong exponent */

    virtual ~Object() {};
    virtual float intersect(Ray ray) const = 0;
    virtual void repr() const = 0;
    virtual cv::Vec3f normalVector(cv::Vec3f point) const = 0;
};

class Sphere : public Object {
private:
    cv::Vec3f center;
    float radius;
public:
    Sphere(cv::Vec3f center, float radius
            , cv::Vec3b color = cv::Vec3b(255,255,255)
            , cv::Vec3b ks = cv::Vec3b(255,255,255)
            , cv::Vec3b ka = cv::Vec3b(255, 255, 255)
            , int p = 100);
    float intersect(Ray ray) const override;
    void repr() const override;
    cv::Vec3f normalVector(cv::Vec3f point) const override;
};


#endif //RAYTRACING_OBJECT_H