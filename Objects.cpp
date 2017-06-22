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
    double t = -((d + n.ddot(ray.origin)) / (n.ddot(ray.direction)));
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

double Rectangle::intersect(Ray ray) const {
    double t = Plane::intersect(ray);
    if (t < 0) {
        return -1;
    }

    cv::Vec3d intersection = ray.origin + ray.direction * t;
    cv::Vec3d vec = intersection - corner;

    double x1 = edge1[0];
    double x2 = edge2[0];
    double y1 = edge1[1];
    double y2 = edge2[1];

    double a, b;
    double delta = x1*y2-x2*y1;

    a = y2*vec[0] - x2*vec[1];
    b = -y1*vec[0] + x1*vec[1];

    if (delta == 0) {
        x1 = edge1[1];
        x2 = edge2[1];
        y1 = edge1[2];
        y2 = edge2[2];
        delta = x1*y2 - x2*y1;
        a = y2*vec[1] - x2*vec[2];
        b = -y1*vec[1] + x1*vec[2];
    }
    if (delta == 0) {
        x1 = edge1[0];
        x2 = edge2[0];
        y1 = edge1[2];
        y2 = edge2[2];
        delta = x1*y2 - x2*y1;
        a = y2*vec[0] - x2*vec[2];
        b = -y1*vec[0] + x1*vec[2];
    }
    assert(delta != 0);
    a /= delta;
    b /= delta;
    if (0 <= a && a <= 1 && 0 <= b && b <= 1) {
        return t;
    } else {
        return -1;
    }
}

Rectangle::Rectangle(cv::Vec3d n, double d, cv::Vec3d corner, cv::Vec3d edge1, cv::Vec3d edge2, Material &m,
                     std::string name) : Plane(n, d, m, name){
    this->corner = corner;
    this->edge1 = edge1;
    this->edge2 = edge2;
}

double Triangle::intersect(Ray ray) const {
    if (!aabb.intersect(ray)) return false;
    auto edge1 = v1 - v0;
    auto edge2 = v2 - v0;
    auto pvec = ray.direction.cross(edge2);
    auto det = edge1.ddot(pvec);

    if (det < 0.00001) return -1;
    auto tvec = ray.origin - v0;
    auto u = tvec.ddot(pvec);
    if (u < 0 || u > det) return -1;
    auto qvec = tvec.cross(edge1);
    double v = ray.direction.dot(qvec);
    if (v < 0 || u+v > det) return -1;
    double t = edge2.ddot(qvec) / det;
    return t;
}

cv::Vec3d Triangle::normalVector(cv::Vec3d point) const {
    // compute plane's normal
    cv::Vec3d v0v1 = v1 - v0;
    cv::Vec3d v0v2 = v2 - v0;
    // no need to normalize
    cv::Vec3d N = cv::normalize(v0v1.cross(v0v2)); // N
//    N = -N/cv::norm(N);
    return N;
}

cv::Vec3d Triangle::getMidPoint() const {
    return (v0 + v1 + v2) / 3;
}

Triangle::Triangle(cv::Vec3d v0, cv::Vec3d v1, cv::Vec3d v2) {
    this->v0 = v0;
    this->v1 = v1;
    this->v2 = v2;
    cv::Vec3d min = v0;
    cv::Vec3d max = v0;
    for (int i = 0; i < 3; ++i) {
        if (v0[i] < min[i]) min[i] = v0[i];
        if (v1[i] < min[i]) min[i] = v1[i];
        if (v2[i] < min[i]) min[i] = v2[i];
        if (v0[i] > max[i]) max[i] = v0[i];
        if (v1[i] > max[i]) max[i] = v1[i];
        if (v2[i] > max[i]) max[i] = v2[i];
    }
    this->aabb = AABB(min, max);
}

void Triangle::repr() const {

}
