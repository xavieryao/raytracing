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
#include <algorithm>
#include <vector>

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

    Color rayTracing(Ray& ray, int depth=5, double epsilon = .0);
    Vec pathTracing(Ray& ray, int depth=5, double epsilon = .0);
    Object* hit(double& t, Ray& ray, double epsilon = .0, double max = INT_MAX);
    static Vec normalize(Vec v);
    static void printVec(Vec& v);
    static void printColor(Color& c);
    static double random() {
        constexpr double HALF_RANGE = 1;
        return -HALF_RANGE + static_cast<double>(rand()) / (RAND_MAX/(HALF_RANGE*2));
    };
    template <class T> static void shuffle(std::vector<T> vec) {
        for (int i = 0; i < vec.size(); i++) {
            auto rnd = i + rand() % (vec.size() - i);
            std::swap(vec[i], vec[rnd]);
        }
    }
    bool refract(Vec& d, Vec&n, double nt, Vec& t);
    static Vec cosineRandomUnitVec(Vec& n) {
        double u1 = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
        double u2 = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
        double r = std::sqrt(u1);
        double theta = 2*M_PI*u2;

        Vec unitVec;
        unitVec[0] = r*std::cos(theta);
        unitVec[1] = r*std::sin(theta);
        unitVec[2] = std::sqrt(std::max(0., 1-u1));
        if (unitVec.ddot(n) < 0) {
            return -unitVec;
        } else {
            return unitVec;
        }
    }

public:
    World(Color bgColor = Color(255,255,255), double aIntensity = 0, std::string name="Brave New World");
    std::string getName() ;
    void setName(std::string name);
    void addObject(Object* obj);
    void addLightSource(Light* l);
    void render(double l, double r, double b, double t, double d, int nx, int ny, Camera& cam, unsigned sampleTimes=9);
    void renderPT(double l, double r, double b, double t, double d, int nx, int ny, Camera& cam, unsigned sampleTimes=9);

    ~World();
};


#endif //RAYTRACING_WORLD_H
