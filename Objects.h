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
#include "AABB.h"


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
    virtual double intersect(Ray ray) const override;
    void repr() const override;
    cv::Vec3d normalVector(cv::Vec3d point) const override;
};


class Plane : public Object {
private:
    cv::Vec3d n;
    double d;

public:
    Plane(cv::Vec3d n, double d, Material& m, std::string name="Untitled");
    virtual double intersect(Ray ray) const override;
    void repr() const override;
    cv::Vec3d normalVector(cv::Vec3d point) const override;
};

class Rectangle : public Plane {
private:
    cv::Vec3d corner, edge1, edge2;
public:
    Rectangle(cv::Vec3d n, double d, cv::Vec3d corner, cv::Vec3d edge1, cv:: Vec3d edge2
            , Material& m, std::string name="Untitled");

    virtual double intersect(Ray ray) const override;
};

class Triangle : public Object {
private:
    cv::Vec3d v0, v1, v2;
public:
    Triangle(cv::Vec3d v0, cv::Vec3d v1, cv::Vec3d v2);
    cv::Vec3d getMidPoint() const;
    virtual double intersect(Ray ray) const override;
    virtual cv::Vec3d normalVector(cv::Vec3d point) const override;
    void repr() const override;
};

#endif //RAYTRACING_OBJECT_H
