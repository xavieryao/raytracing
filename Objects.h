//
// Created by 沛然 姚 on 2017/4/26.
//

#ifndef RAYTRACING_OBJECT_H
#define RAYTRACING_OBJECT_H

#include "Ray.h"
#include "Material.h"
#include <cstdio>
#include <cmath>
#include <algorithm>

/*
 * axis aligned bounding box
 */
struct AABB {
    cv::Vec3d bounds[2];

    AABB() {}

    AABB(cv::Vec3d min, cv::Vec3d max) {
        this->bounds[0] = min;
        this->bounds[1] = max;
    }

    bool intersect(const Ray &r) const
    {
        double tmin, tmax, tymin, tymax, tzmin, tzmax;

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
    static double NO_INTERSECTION;

    Material material;
    AABB aabb;
    std::string type = "Object";
    std::string name = "Untitled";

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
    Sphere(cv::Vec3d center, double radius, Material& mat, std::string name="Untitled");
    double intersect(Ray ray) const override;
    void repr() const override;
    cv::Vec3d normalVector(cv::Vec3d point) const override;
};

class Plane : public Object {
private:
    cv::Vec3d n;
    double d;

public:
    Plane(cv::Vec3d n, double d, Material& m, std::string name="Untitled");
    double intersect(Ray ray) const override;
    void repr() const override;
    cv::Vec3d normalVector(cv::Vec3d point) const override;
};

#endif //RAYTRACING_OBJECT_H
