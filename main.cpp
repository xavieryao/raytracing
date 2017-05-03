#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>


#include "World.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    srand(static_cast<unsigned int>(13488523379));

    Material m1, m2, m3;
    m1.color = Color(100, 123, 5);
    m1.ks = Color(255, 255, 255);
    m1.ka = 0.9*m1.color;
    m1.p = 100;

    m2.color = Color(170, 166, 83);
    m2.ks = Color(255, 255, 255);
    m2.ka = 0.9*m2.color;
    m2.p = 30;

    m3.color = Color(255, 255, 255);
    m3.ks = Color(255, 255, 255);
    m3.ka = Color(255, 255, 255);
    m3.p = 10;

    Material randMaterials[10];
    for (int i = 0; i < 10; ++i) {
        randMaterials[i] = Material::randomMaterial();
    }

    randMaterials[0].color = Vec(255, 0, 0);
    randMaterials[0].ka = randMaterials[0].color*0.9;

    randMaterials[1].color = Vec(0, 255, 0);
    randMaterials[1].ka = randMaterials[1].color*0.9;

    Plane* ground = new Plane(Vec(0, 1, 0), 50, randMaterials[0]);
    Plane* left = new Plane(Vec(1, 0, 0), 50, randMaterials[1]);
    Plane* right = new Plane(Vec(-1, 0, 0), 50, randMaterials[2]);
    Plane* top = new Plane(Vec(0, -1, 0), 50, randMaterials[3]);
    Plane* back = new Plane(Vec(0, 0, -1), 50, randMaterials[1]);

    World w(Color(10, 10, 10), 0.2);
    Light* light = new Light(cv::Vec3f(0, 0, 0));
    Sphere* sp = new Sphere(Vec(0, 0, 20), 15, m1);
    Sphere* another_sp = new Sphere(Vec(9, 13, 40), 25, m2);

    w.addLightSource(light);
//    w.addLightSource(new Light(Vec(-42, 42, 3), 0.8));
//    w.addLightSource(new Light(Vec(-0, 0, 0), 1.0));

//    w.addObject(sp);

    w.addObject(ground);
//    w.addObject(left);
//    w.addObject(right);
//    w.addObject(top);
    w.addObject(back);

    constexpr int size = 10;
    w.render(-35, 35, -35, 35, 3, size, size, Vec(0, 0, 0));
    return 0;
}