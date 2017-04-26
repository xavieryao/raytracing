//
// Created by 沛然 姚 on 2017/4/26.
//

#include "Objects.h"

float Object::NO_INTERSECTION = -3.0f;

float Sphere::intersect(Ray ray) const {
    if (! aabb.intersect(ray)) {
        return NO_INTERSECTION;
    }

    auto& d = ray.direction;
    auto& e = ray.origin;
    auto& c = center;
    auto ec = e-c;
    auto dd = d.ddot(d);
    float delta = static_cast<float>(std::pow(d.ddot(ec), 2) - dd*(ec.ddot(ec) - std::pow(radius, 2)));
    if (delta < 0) {
        return Object::NO_INTERSECTION;
    }

    float sqrt_delta = std::sqrt(delta);
    float t1 = static_cast<float>((-d.ddot(ec)+sqrt_delta)/dd);
    float t2 = static_cast<float>((-d.ddot(ec)-sqrt_delta)/dd);
    float t = std::min(t1, t2);
    if (t < 0) return NO_INTERSECTION;
    return t;
}

Sphere::Sphere(cv::Vec3f center, float radius, cv::Vec3b color, cv::Vec3b ks, cv::Vec3b ka, int p) {
    this->center = center;
    this->radius = radius;
    this->color = color;
    this->ks = ks;
    this->ka = ka;
    this->p = p;
    auto vec_r = cv::Vec3f(radius, radius, radius);
    this->aabb = AABB(center-vec_r, center+vec_r);
}

void Sphere::repr() const {
    printf("(%f, %f, %f) R=%f\n", center[0], center[1], center[2], radius);
}

cv::Vec3f Sphere::normalVector(cv::Vec3f point) const {
    auto vec = point-center;
    return vec/cv::norm(vec);
}
