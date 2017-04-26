#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

#include "World.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    std::cout << (1/.0 > 5) << std::endl;

    World w(Color(59, 66, 131));
    Light* light = new Light(cv::Vec3f(10, 0, 0));
    Sphere* sp = new Sphere(Vec(0, 0, 20), 10, Color(100, 123, 5), Color(255, 255, 255));
    w.addLightSource(light);
    w.addLightSource(new Light(Vec(42, 42, 42), 0.8));
    w.addObject(sp);
    w.render(-15, 15, -15, 15, 3, 4096, 4096);
    return 0;
}