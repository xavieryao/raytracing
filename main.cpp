#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>


#include "World.h"

int main() {

    std::cout << "Hello, World!" << std::endl;
    srand(static_cast<unsigned int>(534599119));


    Color c = Color(100, 123, 5);
    Vec cv = World::color2vec(c);
    cv *= 5;
    World::printColor(cv);

    Material m1;
    m1.color = Color(100, 123, 5);
    m1.ks = Color(230, 230, 230);
    m1.ka = 0.9*m1.color;
    m1.p = 100;
    m1.dielectric = true;
    m1.ab = 0.00001;
    m1.ag = 0.00001;
    m1.ar = 0.00001;
    m1.nt = 1.17;

    Material m2;
    m2.color = Color(100, 123, 5);
    m2.ks = Color(230, 230, 230);
    m2.ka = 0.9*m1.color;
    m2.p = 100;
    m2.dielectric = true;
    m2.ab = 0.1;
    m2.ag = 0.00001;
    m2.ar = 0.00001;
    m2.nt = 1.17;

    Material randMaterials[10];
    for (int i = 0; i < 10; ++i) {
        randMaterials[i] = Material::randomMaterial();
    }

    Material mirmat = randMaterials[1];
    mirmat.km = 0.4;
    mirmat.color = Color(73,85,132);



    randMaterials[0].color = Color(0, 140, 255);
    randMaterials[0].p = 200;
    randMaterials[1].color = Color(200, 30, 40);
    randMaterials[2].color = Color(20, 30, 180);
    randMaterials[4].color = Color(150, 40, 180);
    randMaterials[6].color = Color(88, 102, 53);


    Plane* ground = new Plane(Vec(0, 1, 0), 10, randMaterials[0], "ground");
    Plane* left = new Plane(Vec(1, 0, 0), 50, randMaterials[1], "left");
    Plane* right = new Plane(Vec(-1, 0, 0), 50, randMaterials[2], "right");
    Plane* top = new Plane(Vec(0, -1, 0), 13, randMaterials[4], "top");
    Plane* back = new Plane(Vec(0, 0, -1), 30, randMaterials[6], "back");

    randMaterials[5].emission = Vec(30, 30, 30);
    randMaterials[5].color = Vec(0, 0, 0);
    Rectangle* led = new Rectangle(Vec(0, -1, 0), 13, Vec(-6, 13, 4), Vec(6, 0, 0), Vec(0, 0, 6), randMaterials[5], "led");
    Rectangle* led2 = new Rectangle(Vec(0, -1, 0), 13, Vec(6, 13, 5), Vec(-6, 0, 0), Vec(0, 0, -6), randMaterials[5], "led");


    World w(Color(10, 10, 10), 0.5);
    Light* light = new Light(cv::Vec3d(-8, 10, -2), cv::Vec3d(-0.6, 0.6, 0.6), cv::Vec3d(0.75, 1, 1), 1.5);

    Sphere* sp = new Sphere(Vec(0, -1 , 0), 2, m1, "sp");
    Sphere* anotherSp = new Sphere(Vec(3, 3, 5), 3, mirmat, "another_sp");
    Sphere* sp2 = new Sphere(Vec(-2, -1 , -3), 0.5, m1, "sp");

    Material material = randMaterials[9];
    material.km = 0;
    Sphere* distantSp = new Sphere(Vec(-8, 8, 20), 5, material, "distantSp");

    auto tris = Triangle::loadMeshes("/Users/xavieryao/tmp/mesh_full.obj", Vec(2, 0, 0), 0.015, material);

    auto kdtree = KDNode::build(tris, 0);


    w.addLightSource(light);
//    w.addLightSource(light2);

    w.addObject(sp);
//    w.addObject(sp2);
    w.addObject(anotherSp);
    w.addObject(distantSp);
    w.addObject(led);
    w.addObject(top);
    w.addObject(back);
    w.addObject(left);
    w.addObject(right);
    w.addObject(ground);

//    w.addObject(triangle);

    w.addKDTree(kdtree);

    constexpr int size = 1000;

    constexpr int frame = 17;

    Camera cam(Vec(0,-1.6,-4));
    cam.pitch(-10);

    cam.focus = 0.08;


    /*
    for (int i = 0; i < tris.size(); i++) {
        printf("tris %d\n", i);
        w.addObject(tris[i]);
        w.setName(std::to_string(i));
        w.render(-frame, frame, -frame, frame, 9, size, size, cam, 1);
    }*/

    w.renderPT(-frame, frame, -frame, frame, 9, size, size, cam, 50000);


    return 0;
}