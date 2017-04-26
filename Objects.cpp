//
// Created by 沛然 姚 on 2017/4/26.
//

#include "Objects.h"

double Object::NO_INTERSECTION = -3;

double Sphere::intersect(Ray ray) const {
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
    double t = std::min(t1, t2);
    if (t < 0) return NO_INTERSECTION;
    return t;
}

Sphere::Sphere(cv::Vec3d center, double radius, cv::Vec3b color, cv::Vec3b ks, cv::Vec3b ka, int p) {
    this->center = center;
    this->radius = radius;
    this->color = color;
    this->ks = ks;
    this->ka = ka;
    this->p = p;
}

void Sphere::repr() const {
    printf("(%f, %f, %f) R=%f\n", center[0], center[1], center[2], radius);
}

cv::Vec3d Sphere::normalVector(cv::Vec3d point) const {
    auto vec = point-center;
    return vec/cv::norm(vec);
}
