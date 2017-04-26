//
// Created by 沛然 姚 on 2017/4/26.
//

#include "Objects.h"

double Object::NO_INTERSECTION = -3;

double Sphere::intersect(Ray ray) {
    auto& d = ray.direction;
    auto& e = ray.origin;
    auto& c = center;
    auto ec = e-c;
    auto dd = d.mul(d);
    auto delta = pow(d.mul(ec), 2) - dd*(ec.mul(ec) - pow(radius, 2));
    if (delta < 0)
        return Object::NO_INTERSECTION;

    auto t1 = (-d.mul(ec)+std::sqrt(delta))/d.mul(d);
    auto t2 = (-d.mul(ec)+std::sqrt(delta))/d.mul(d);
    auto t = std::min(t1, t2);
    if (t < 0) return NO_INTERSECTION;
    return t;
}

Sphere::Sphere(double x, double y, double z, double radius, cv::Vec3b color, cv::Vec3b ks, int p) {
    this->center = cv::Vec3d(x, y, z);
    this->radius = radius;
    this->color = color;
    this->ks = ks;
    this->p = p;
}

void Sphere::repr() {
    printf("(%f, %f, %f) R=%f\n", center[0], center[1], center[2], radius);
}
