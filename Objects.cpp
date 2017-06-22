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
    cv::Vec3d edge1 = v1 - v0;
    cv::Vec3d edge2 = v2 - v0;

    cv::Vec3d triNorm = normalVector(ray.direction.cross(edge2));
    double vn = ray.direction.ddot(triNorm);

    cv::Vec3d aa = ray.origin - v0;
    double xpn = aa.ddot(triNorm);
    double t = -xpn/vn;

    if (t < 0.0000001) {
        return -1;
    }

    cv::Vec3d hitPos = ray.origin + t*ray.direction;
    cv::Vec3d hit00 = hitPos - v0;
    cv::Vec3d cross0 = hit00.cross(edge1);
    if (cross0.ddot(triNorm) > 0.00000001) return -1;

    cv::Vec3d hit10 = hitPos - v1;
    cv::Vec3d hit11 = v2 - v1;
    cv::Vec3d cross1 = hit10.cross(hit11);
    if (cross1.dot(triNorm) > 0.00000001) return -1;

    cv::Vec3d hit20 = hitPos - v2;
    cv::Vec3d hit21 = v0 - v2;
    cv::Vec3d cross2 = hit20.cross(hit21);
    if (cross2.ddot(triNorm) > 0.00000001) return -1;


    cv::Vec3d P = ray.origin + t*ray.direction;

    if (t > 0) {
        printf("%f %f %f %f\n", P[0], P[1], P[2], t);
//        assert(aabb.intersect(ray));
    }
    return t;
}

cv::Vec3d Triangle::normalVector(cv::Vec3d point) const {
    // compute plane's normal
    cv::Vec3d v0v1 = v1 - v0;
    cv::Vec3d v0v2 = v2 - v0;
    // no need to normalize
    cv::Vec3d N = v0v1.cross(v0v2); // N
    N = N/cv::norm(N);
    return N;
}

cv::Vec3d Triangle::getMidPoint() const {
    return (v0 + v1 + v2) / 3;
}

Triangle::Triangle(cv::Vec3d v0, cv::Vec3d v1, cv::Vec3d v2) {
    this->v0 = v0;
    this->v1 = v1;
    this->v2 = v2;
    cv::Vec3d min = cv::Vec3d(std::min(v0[0], std::min(v1[0], v2[0])), std::min(v0[1], std::min(v1[1], v2[1]))
            , std::min(v0[2], std::min(v1[2], v2[2])));
    cv::Vec3d max = cv::Vec3d(std::max(v0[0], std::max(v1[0], v2[0])), std::max(v0[1], std::max(v1[1], v2[1]))
            , std::max(v0[2], std::max(v1[2], v2[2])));

//    min = cv::Vec3d(4, 2, 1);
//    max = cv::Vec3d(6, 7, 8);
    this->aabb = AABB(min, max);
}

void Triangle::repr() const {

}
