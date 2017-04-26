#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

#include "World.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    World w(Color(59, 66, 131), 0.1);
    Light* light = new Light(cv::Vec3f(10, 0, 0));
    Sphere* sp = new Sphere(Vec(0, 0, 20), 10, Color(100, 123, 5), Color(255, 255, 255));
    Sphere* another_sp = new Sphere(Vec(9, 13, 40), 25, Color(170, 166, 83), Color(255, 255, 255));
    w.addLightSource(light);
    w.addLightSource(new Light(Vec(-42, 42, 3), 0.8));
    w.addLightSource(new Light(Vec(-10, 0, 0), 0.8));
    w.addObject(sp);
    w.addObject(another_sp);
    w.render(-15, 15, -15, 15, 3, 2048, 2048);
    return 0;
}