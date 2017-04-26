//
// Created by 沛然 姚 on 2017/4/26.
//

#ifndef RAYTRACING_WORLD_H
#define RAYTRACING_WORLD_H

#include "Objects.h"
#include "Light.h"
#include <vector>

class World {
private:
    std::vector<Object> objects;
    std::vector<Light> lightSources;

public:
    void addObject(Object& obj);
    void addLightSource(Light& l);
    void render(cv::Vec3d eye, double l, double r, double b, double t, double d) const;
};


#endif //RAYTRACING_WORLD_H
