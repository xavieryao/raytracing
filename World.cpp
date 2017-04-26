//
// Created by 沛然 姚 on 2017/4/26.
//

#include "World.h"
#include "tqdm/tqdm.h"


void World::addObject(Object* obj) {
    objects.push_back(obj);
}

/*
 * l,r,b,t: describes view (how broad you can see)
 * d: view depth
 * nx, ny: height and width
 */
void World::render(double l, double r, double b, double t, double d, int nx, int ny, cv::Vec3d eye, cv::Scalar bgColor) const {
    auto image = cv::Mat(nx, ny, CV_8UC3, bgColor);

    auto uu = Vec(1, 0, 0);
    auto vv = Vec(0, 1, 0);
    auto ww = Vec(0, 0, -1);

    
    cv::imshow(name, image);
    cv::waitKey();
}

void World::addLightSource(Light* l) {
    lightSources.push_back(l);
}

World::~World() {
    for (auto light : lightSources) {
        delete light;
    }
    for (auto obj: objects) {
        delete obj;
    }
}

void World::setName(std::string name) {
    this->name = name;
}

std::string World::getName() const {
    return name;
}

World::World(std::string name) {
    this->name = name;
}
