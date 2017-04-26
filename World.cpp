//
// Created by 沛然 姚 on 2017/4/26.
//

#include "World.h"

void World::addObject(Object &obj) {
    objects.push_back(obj);
}

/*
 * l,r,b,t: describes view
 * d: view depth
 */
void World::render(cv::Vec3d eye, double l, double r, double b, double t, double d) const {

}

void World::addLightSource(Light &l) {
    lightSources.push_back(l);
}
