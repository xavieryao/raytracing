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
typedef cv::Vec3d Vec;

class World {
private:
    bool verbose;

    std::vector<Object*> objects;
    std::vector<Light*> lightSources;
    std::string name;
    Color bgColor;
    double aIntensity;

    Color rayTracing(Ray& ray, int depth=7, double epsilon = .0) ;
    Object* hit(double& t, Ray& ray, double epsilon = .0, double max = INT_MAX) ;
    static Vec normalize(Vec v);
    static void printVec(Vec& v);
    static void printColor(Color& c);
    bool refract(Vec& d, Vec&n, double nt, Vec& t) ;
    void render(double l, double r, double b, double t, double d, int nx, int ny, Camera& cam, cv::Mat& mat);
    Color superSample(int i, int j, int nx, int ny, cv::Mat &image) ;

public:
    World(Color bgColor = Color(255,255,255), double aIntensity = 0, std::string name="Brave New World");
    std::string getName() ;
    void setName(std::string name);
    void addObject(Object* obj);
    void addLightSource(Light* l);
    void render(double l, double r, double b, double t, double d, int nx, int ny,
                Camera& cam, bool ssaa=false) ;
    ~World();
};


#endif //RAYTRACING_WORLD_H
