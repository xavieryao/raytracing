#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>


#include "World.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    srand(static_cast<unsigned int>(3847524325));

    Material m1;
    m1.color = Color(100, 123, 5);
    m1.ks = Color(230, 230, 230);
    m1.ka = 0.9*m1.color;
    m1.p = 100;

    Material randMaterials[10];
    for (int i = 0; i < 10; ++i) {
        randMaterials[i] = Material::randomMaterial();
    }

    randMaterials[0].km = 0.7;
    randMaterials[0].color = Color(20, 20, 20);
    randMaterials[0].ka = Color(20, 20, 20);
    randMaterials[0].ks = Color(240, 240, 240);

//    randMaterials[1].km = 0.4;

    Plane* ground = new Plane(Vec(0, 1, 0), 20, randMaterials[0], "ground");
    Plane* left = new Plane(Vec(1, 0, 0), 50, randMaterials[1], "left");
    Plane* right = new Plane(Vec(-1, 0, 0), 50, randMaterials[2], "right");
    Plane* top = new Plane(Vec(0, -1, 0), 20, randMaterials[3], "top");
    Plane* back = new Plane(Vec(0, 0, -1), 10, randMaterials[4], "back");

    World w(Color(10, 10, 10), 0.2);
//    w.setName("ssaa");
    Light* light = new Light(cv::Vec3f(0, 10, -2));
    Light* light2 = new Light(cv::Vec3f(12, 10, -2));
    Light* light3 = new Light(cv::Vec3f(0, 0, -2), 1);
    Light* light4 = new Light(cv::Vec3f(-3, -3, -2));

    Sphere* sp = new Sphere(Vec(0, -7 , 3.8), 3.5, m1);
    randMaterials[5].km = 0.3;
//    Sphere* another_sp = new Sphere(Vec(0, -19, 1.0), 0.8, randMaterials[5]);

    w.addLightSource(light);
    w.addLightSource(light2);
    w.addLightSource(light3);
    w.addLightSource(light4);

    w.addObject(sp);
//    w.addObject(another_sp);

    w.addObject(ground);
    w.addObject(left);
    w.addObject(right);
    w.addObject(top);
    w.addObject(back);

    constexpr int size = 400;
    constexpr int frame = 50;

    Camera cam(Vec(0, -10, -2));


    for (int i = -15; i < 15; ++i) {
        cam.pitch(0.3*i);
        w.setName(std::to_string(i+15));
        w.render(-frame, frame, -frame, frame, 7, size, size, cam, false);
        cam.pitch(-0.3*i);
    }

    return 0;
}