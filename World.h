//
// Created by 沛然 姚 on 2017/4/26.
//

#ifndef RAYTRACING_WORLD_H
#define RAYTRACING_WORLD_H

#include "Objects.h"
#include "Light.h"
#include "Camera.h"
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

    Color rayTracing(Ray& ray, int depth=5, float epsilon = .0) const;
    Object* hit(float& t, Ray& ray, float epsilon = .0, double max = INT_MAX) const;
    static Vec normalize(Vec v);
    static void printVec(Vec& v);
    static void printColor(Color& c);
    bool refract(Vec& d, Vec&n, float nt, Vec& t) const;
    void render(float l, float r, float b, float t, float d, int nx, int ny, Camera& cam, cv::Mat& mat) const;
    Color superSample(int i, int j, int nx, int ny, cv::Mat &image) const;

public:
    World(Color bgColor = Color(255,255,255), float aIntensity = 0, std::string name="Brave New World");
    std::string getName() const;
    void setName(std::string name);
    void addObject(Object* obj);
    void addLightSource(Light* l);
    void render(float l, float r, float b, float t, float d, int nx, int ny,
                Camera& cam, bool ssaa=false) const;
    ~World();
};


#endif //RAYTRACING_WORLD_H
