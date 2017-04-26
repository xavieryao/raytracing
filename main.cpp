#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

#include "World.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    Material m1, m2, m3;
    m1.color = Color(100, 123, 5);
    m1.ks = Color(255, 255, 255);
    m1.ka = 0.9*m1.color;
    m1.p = 100;

    m2.color = Color(170, 166, 83);
    m2.ks = Color(255, 255, 255);
    m2.ka = 0.9*m2.color;
    m2.p = 30;

    World w(Color(59, 66, 131), 0.1);
    Light* light = new Light(cv::Vec3f(10, 0, 0));
    Sphere* sp = new Sphere(Vec(0, 0, 20), 10, m1);
    Sphere* another_sp = new Sphere(Vec(9, 13, 40), 25, m2);
    w.addLightSource(light);
    w.addLightSource(new Light(Vec(-42, 42, 3), 0.8));
    w.addLightSource(new Light(Vec(-10, 0, 0), 0.8));
    w.addObject(sp);
    w.addObject(another_sp);
    w.render(-15, 15, -15, 15, 3, 2048, 2048);
    return 0;
}