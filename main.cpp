#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>


#include "World.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
//    srand(static_cast<unsigned int>(13488523379));
    srand(static_cast<unsigned int>(3847524325));

    Material m1;
    m1.color = Color(100, 123, 5);
    m1.ks = Color(255, 255, 255);
    m1.ka = 0.9*m1.color;
    m1.p = 100;

    auto m2 = Material::randomMaterial();

    Material randMaterials[10];
    for (int i = 0; i < 10; ++i) {
        randMaterials[i] = Material::randomMaterial();
    }

    Plane* ground = new Plane(Vec(0, 1, 0), 50, randMaterials[0], "ground");
    Plane* left = new Plane(Vec(1, 0, 0), 50, randMaterials[1], "left");
    Plane* right = new Plane(Vec(-1, 0, 0), 50, randMaterials[2], "right");
    Plane* top = new Plane(Vec(0, -1, 0), 50, randMaterials[3], "top");
    Plane* back = new Plane(Vec(0, 0, -1), 20, randMaterials[4], "back");

    World w(Color(10, 10, 10), 0.2);
    w.setName("ssaa");
    Light* light = new Light(cv::Vec3f(0, 10, 0));
    Light* light2 = new Light(cv::Vec3f(20, 10, 0));
    Light* light3 = new Light(cv::Vec3f(0, 0, 0), 1);

    Sphere* sp = new Sphere(Vec(0, 0, 30), 25, m1);
    Sphere* another_sp = new Sphere(Vec(0.1, 0.1, 1.5), 0.8, randMaterials[5]);

    w.addLightSource(light);
    w.addLightSource(light2);
    w.addLightSource(light3);

//    w.addLightSource(new Light(cv::Vec3f(42, 42, 5), 0.8));
//    w.addLightSource(new Light(cv::Vec3f(13, -9, 9), 0.7));

    w.addObject(sp);
    w.addObject(another_sp);

    w.addObject(ground);
    w.addObject(left);
    w.addObject(right);
    w.addObject(top);
    w.addObject(back);

    constexpr int size = 500;
    w.render(-35, 35, -35, 35, 10, size, size, Vec(0, 0, 0), true);
    return 0;
}