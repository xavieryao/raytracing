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
    m1.dielectric = true;
    m1.ab = 0.00001;
    m1.ag = 0.00001;
    m1.ar = 0.00001;
    m1.nt = 1.05;

    Material randMaterials[10];
    for (int i = 0; i < 10; ++i) {
        randMaterials[i] = Material::randomMaterial();
    }

    randMaterials[1].km = 0.8;
//    randMaterials[0].color = Color(0, 0, 0);
//    randMaterials[0].ka = Color(0, 0, 0);
//    randMaterials[0].ks = Color(140, 140, 140);


//    randMaterials[0].km = 0.5;
//    randMaterials[0].p = 20;
//    randMaterials[3].km = 0.5;

    Plane* ground = new Plane(Vec(0, 1, 0), 10, randMaterials[0], "ground");
    Plane* left = new Plane(Vec(1, 0, 0), 50, randMaterials[1], "left");
    Plane* right = new Plane(Vec(-1, 0, 0), 50, randMaterials[2], "right");
    Plane* top = new Plane(Vec(0, -1, 0), 15, randMaterials[3], "top");
    Plane* back = new Plane(Vec(0, 0, -1), 10, randMaterials[4], "back");

    World w(Color(10, 10, 10), 0.5);
    Light* light = new Light(cv::Vec3d(-8, 10, -2), cv::Vec3d(-0.03, 0.02, 0.02), cv::Vec3d(0.015, 0.02, 0.02), 1.5);
    Light* light2 = new Light(cv::Vec3d(8, 10, 2), cv::Vec3d(0.12, 0.12, 0.12), cv::Vec3d(0.023, 0.02, 0.02), 1.5);
   // Light* light3 = new Light(cv::Vec3d(0, 0, -2), 0.5);
//    Light* light4 = new Light(cv::Vec3d(-3, -3, -2));

    Sphere* sp = new Sphere(Vec(0, -1 , 0), 3, m1, "sp");
    Sphere* anotherSp = new Sphere(Vec(3, 3, 5), 4, randMaterials[1], "another_sp");
    Sphere* distantSp = new Sphere(Vec(-8, 8, 25), 5, randMaterials[1], "distantSp");
    randMaterials[5].km = 0.3;

    w.addLightSource(light);
    w.addLightSource(light2);
//    w.addLightSource(light3);
//    w.addLightSource(light4);

    w.addObject(sp);
    w.addObject(anotherSp);
    w.addObject(distantSp);

    w.addObject(ground);
//    w.addObject(left);
//    w.addObject(right);
//    w.addObject(top);
//    w.addObject(back);

    constexpr int size = 2000;
    constexpr int frame = 15;

    Camera cam(Vec(0, 1, -5));
    cam.pitch(10);

    double focus = 0.00001;
    w.render(-frame, frame, -frame, frame, 9, size, size, cam, 24);

    /*
    for (int i = 0; i < 15; i++) {
        cam.focus = focus;
        w.setName(std::to_string(focus));
        w.render(-frame, frame, -frame, frame, 9, size, size, cam, 3);
        focus *= 2;
    }
     */

    return 0;
}