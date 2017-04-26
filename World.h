//
// Created by 沛然 姚 on 2017/4/26.
//

#ifndef RAYTRACING_WORLD_H
#define RAYTRACING_WORLD_H

#include "Objects.h"
#include "Light.h"
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <string>

typedef cv::Vec3b Color;
typedef cv::Vec3f Vec;

class World {
private:
    std::vector<Object*> objects;
    std::vector<Light*> lightSources;
    std::string name;
    Color bgColor;
    float aIntensity;

    Color rayTracing(Ray& ray) const;
    static Vec normalize(Vec v);

public:
    World(Color bgColor = Color(255,255,255), float aIntensity = 0, std::string name="Brave New World");
    std::string getName() const;
    void setName(std::string name);
    void addObject(Object* obj);
    void addLightSource(Light* l);
    void render(float l, float r, float b, float t, float d, int nx = 512, int ny = 512,
                cv::Vec3f eye = cv::Vec3f(0, 0, 0)) const;
    ~World();
};


#endif //RAYTRACING_WORLD_H