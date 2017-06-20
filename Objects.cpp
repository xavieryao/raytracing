//
// Created by 沛然 姚 on 2017/4/26.
//

#include "Objects.h"

double Object::NO_INTERSECTION = -3.0f;

double Sphere::intersect(Ray ray) const {
    if (! aabb.intersect(ray)) {
        return NO_INTERSECTION;
    }

    auto& d = ray.direction;
    auto& e = ray.origin;
    auto& c = center;
    auto ec = e-c;
    auto dd = d.ddot(d);
    double delta = std::pow(d.ddot(ec), 2) - dd*(ec.ddot(ec) - std::pow(radius, 2));
    if (delta < 0) {
        return Object::NO_INTERSECTION;
    }

    double sqrt_delta = std::sqrt(delta);
    double t1 = (-d.ddot(ec)+sqrt_delta)/dd;
    double t2 = (-d.ddot(ec)-sqrt_delta)/dd;
    return t2 < 0.0001 ? t1 : t2;
}

Sphere::Sphere(cv::Vec3d center, double radius, Material& mat, std::string name) {
    this->center = center;
    this->radius = radius;
    this->material = mat;
    this->name = name;
    auto vec_r = cv::Vec3d(radius, radius, radius);
    this->aabb = AABB(center-vec_r, center+vec_r);
    this->type = "sphere";
}

void Sphere::repr() const {
    printf("(%.2f, %.2f, %.2f) R=%.2f\n", center[0], center[1], center[2], radius);
}

cv::Vec3d Sphere::normalVector(cv::Vec3d point) const {
    auto vec = point-center;
    return vec/cv::norm(vec);
}

double Plane::intersect(Ray ray) const {
    double t = -static_cast<double>((d + n.ddot(ray.origin))/(n.ddot(ray.direction)));
    return t;
}

Plane::Plane(cv::Vec3d n, double d, Material& m, std::string name) {
    this->type = "Plane";
    this->n = n/cv::norm(n);
    this->d = d;
    this->material = m;
    this->name = name;
}

void Plane::repr() const {
    printf("%s N: (%.2f, %.2f, %.2f) D: %f \n", name.c_str(), n[0], n[1], n[2], d);
}

cv::Vec3d Plane::normalVector(cv::Vec3d point) const {
    return n;
}
