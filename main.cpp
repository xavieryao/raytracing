#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>


#include "World.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
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


    randMaterials[0].km = 1;
    randMaterials[0].color = Color(20, 20, 20);
    randMaterials[0].ka = Color(20, 20, 20);
    randMaterials[0].ks = Color(20, 20, 20);
//    randMaterials[3].km = Color(150, 150, 150);
//    randMaterials[2].km = Color(150, 150, 150);
//    randMaterials[1].km = Color(150, 150, 150);
//    randMaterials[4].km = Color(150, 150, 150);

//    randMaterials[3].km = 0.6;

    Plane* ground = new Plane(Vec(0, 1, 0), 20, randMaterials[0], "ground");
    Plane* left = new Plane(Vec(1, 0, 0), 50, randMaterials[1], "left");
    Plane* right = new Plane(Vec(-1, 0, 0), 50, randMaterials[2], "right");
    Plane* top = new Plane(Vec(0, -1, 0), 20, randMaterials[3], "top");
    Plane* back = new Plane(Vec(0, 0, -1), 10, randMaterials[4], "back");

    World w(Color(10, 10, 10), 0.2);
//    w.setName("ssaa");
    Light* light = new Light(cv::Vec3f(0, 10, 0));
    Light* light2 = new Light(cv::Vec3f(20, 10, 0));
    Light* light3 = new Light(cv::Vec3f(0, 0, 0), 1);

    Sphere* sp = new Sphere(Vec(0, -15 ,4.8), 4.5, m1);
    randMaterials[5].km = 0.3;
//    Sphere* another_sp = new Sphere(Vec(0, -19, 1.0), 0.8, randMaterials[5]);

    w.addLightSource(light);
    w.addLightSource(light2);
    w.addLightSource(light3);

    w.addObject(sp);
//    w.addObject(another_sp);

    w.addObject(ground);
    w.addObject(left);
    w.addObject(right);
    w.addObject(top);
    w.addObject(back);

    constexpr int size = 400;
    constexpr int frame = 50;

    Camera cam(Vec(0, 0, -3));

    for (int i = -15; i < 15; ++i) {
        cam.yaw(0.3*i);
        w.setName(std::to_string(i));
        w.render(-frame, frame, -frame, frame, 5, size, size, cam, false);
        cam.yaw(-0.3*i);
    }

    return 0;
}