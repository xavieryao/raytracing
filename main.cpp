#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>


#include "World.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    srand(static_cast<unsigned int>(534599119));


    Material m1;
    m1.color = Color(100, 123, 5);
    m1.ks = Color(230, 230, 230);
    m1.ka = 0.9*m1.color;
    m1.p = 100;
    m1.dielectric = false;
    m1.ab = 0.00001;
    m1.ag = 0.00001;
    m1.ar = 0.00001;
    m1.nt = 1.07;

    Material randMaterials[10];
    for (int i = 0; i < 10; ++i) {
        randMaterials[i] = Material::randomMaterial();
    }

    Material mirmat = randMaterials[1];
    mirmat.km = 0.4;
    mirmat.color = Color(73,85,132);



    randMaterials[0].color = Color(0, 140, 255);
    randMaterials[1].color = Color(200, 30, 40);
    randMaterials[2].color = Color(20, 30, 180);
    randMaterials[4].color = Color(150, 40, 180);
    randMaterials[6].color = Color(88, 102, 53);


    Plane* ground = new Plane(Vec(0, 1, 0), 10, randMaterials[0], "ground");
    Plane* left = new Plane(Vec(1, 0, 0), 50, randMaterials[1], "left");
    Plane* right = new Plane(Vec(-1, 0, 0), 50, randMaterials[2], "right");
    Plane* top = new Plane(Vec(0, -1, 0), 13, randMaterials[4], "top");
    Plane* back = new Plane(Vec(0, 0, -1), 30, randMaterials[6], "back");

    randMaterials[5].emission = Vec(2800, 2800, 2800);
    Rectangle* led = new Rectangle(Vec(0, -1, 0), 13, Vec(-6, 13, 4), Vec(6, 0, 0), Vec(0, 0, 6), randMaterials[5], "led");
    Rectangle* led2 = new Rectangle(Vec(0, -1, 0), 13, Vec(6, 13, 5), Vec(-6, 0, 0), Vec(0, 0, -6), randMaterials[5], "led");


    World w(Color(10, 10, 10), 0.5);
    Light* light = new Light(cv::Vec3d(-8, 10, -2), cv::Vec3d(-0.03, 0.02, 0.02), cv::Vec3d(0.015, 0.02, 0.02), 1.5);
    Light* light2 = new Light(cv::Vec3d(8, 10, -2), cv::Vec3d(0.12, 0.12, 0.12), cv::Vec3d(0.023, 0.02, 0.02), 1.5);

    Sphere* sp = new Sphere(Vec(0, -1 , 0), 3, m1, "sp");
    Sphere* anotherSp = new Sphere(Vec(3, 3, 5), 4, mirmat, "another_sp");

    Material material = randMaterials[9];
    material.km = 0;
    Sphere* distantSp = new Sphere(Vec(-8, 8, 20), 5, material, "distantSp");

    w.addLightSource(light);
    w.addLightSource(light2);

    w.addObject(sp);
    w.addObject(anotherSp);
    w.addObject(distantSp);
    w.addObject(led);
    w.addObject(top);
    w.addObject(back);
    w.addObject(left);
    w.addObject(right);
    w.addObject(ground);

    constexpr int size = 200;
    constexpr int frame = 23;

    Camera cam(Vec(0, 1, -5));
    cam.pitch(10);

    double focus = 0.001;
    w.renderPT(-frame, frame, -frame, frame, 9, size, size, cam, 20);

    /*
    for (int i = 0; i < 25; i++) {
        cam.focus = focus;
        w.setName(std::to_string(focus));
        w.render(-frame, frame, -frame, frame, 9, size, size, cam, 16);
        focus *= 2;
    }
     */

    return 0;
}