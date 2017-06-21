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
#include <cstdlib>

typedef cv::Vec3b Color;
typedef cv::Vec3d Vec;

class World {
private:
    std::vector<Object*> objects;
    std::vector<Light*> lightSources;
    std::string name;
    Color bgColor;
    double aIntensity;

    static Color rayTracing(Ray& ray, Color& bgColor, std::vector<Object*>& objects, std::vector<Light*>& lightSource
            , double aIntensity, int depth=5, double epsilon = .0);
    static Object* hit(double& t, Ray& ray, std::vector<Object*>& objects, double epsilon = .0, double max = INT_MAX);
    static Vec normalize(Vec v);
    static void printVec(Vec& v);
    static void printColor(Color& c);
    static double random() {
        constexpr double HALF_RANGE = 1.5;
        return -HALF_RANGE + static_cast<double>(rand()) / (RAND_MAX/(HALF_RANGE*2));
    };
    static bool refract(Vec& d, Vec&n, double nt, Vec& t);

public:
    World(Color bgColor = Color(255,255,255), double aIntensity = 0, std::string name="Brave New World");
    std::string getName() ;
    void setName(std::string name);
    void addObject(Object* obj);
    void addLightSource(Light* l);
    void render(double l, double r, double b, double t, double d, int nx, int ny, Camera& cam, int sampleTimes=9);

    ~World();
};


#endif //RAYTRACING_WORLD_H
