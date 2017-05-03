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
void World::render(float l, float r, float b, float t, float d, int nx, int ny, cv::Vec3f eye) const {
    auto image = cv::Mat(nx, ny, CV_8UC3, cv::Scalar(bgColor[0], bgColor[1], bgColor[2]));

    std::cout << "render" << std::endl;
    auto uu = Vec(1, 0, 0);
    auto vv = Vec(0, -1, 0);
    auto ww = Vec(0, 0, -1);

    for (int i = 0; i < ny; ++i) {
        printf("column %d\n", i);
        for (int j = 0; j < nx; ++j) {
            printf("\n");

            auto u = l + (r - l) * (i + 0.5) / ny;
            auto v = b + (t - b) * (j + 0.5) / nx;
            auto direction = -d*ww+u*uu+v*vv;
            auto ray = Ray(eye, direction);
            image.at<Color>(j, i) = rayTracing(ray);
        }
    }
//    cv::imshow(name, image);
//    cv::waitKey();
    auto filename = "/Users/xavieryao/cg/rt/"+name+".png";
    cv::imwrite(filename, image);
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

World::World(Color bgColor, float aIntensity, std::string name) {
    this->name = name;
    this->aIntensity = aIntensity;
    this->bgColor = bgColor;
}

Color World::rayTracing(Ray& ray) const {
#ifndef NDEBUG
    printf("ray tracing: Ray origin: %.2f %.2f %.2f, Ray direction: %.2f %.2f %.2f \n",ray.origin[0], ray.origin[1],
           ray.origin[2], ray.direction[0], ray.direction[1], ray.direction[2]);
#endif
    float t;
    Object* object = hit(t, ray);
    if (object == nullptr) {
#ifndef NDEBUG
        printf("no object hit.\n");
#endif
        return bgColor;
    }

    Vec intersection = ray.origin + t*ray.direction;
    Vec n = object->normalVector(intersection);
    Vec v = normalize(ray.origin - intersection);

#ifndef NDEBUG
    printf("intersection: ");
    printVec(intersection);
    printf("n: ");
    printVec(n);
    printf("v: ");
    printVec(v);
#endif

    Color color = object->material.ka * aIntensity;
    for (Light* light: lightSources) {

        Vec lt = normalize(light->position - intersection);
        Ray shadowRay(intersection, lt);

#ifndef NDEBUG
        printf("for light ");
        light->repr();
        printf("lt: "); printVec(lt);
#endif

        float s;
        if (!hit(s, shadowRay, 0.01)) {
            Vec h = normalize(v + lt);
            double a = n.ddot(lt);
            double b = n.ddot(h);
#ifndef NDEBUG
            printf("h: "); printVec(h);
            printf("color coefficient: %.2f  Ks coefficient %.2f\n", a, b);
#endif
            color += object->material.color * light->intensity * std::max(0.0, a);
            color += object->material.ks * light->intensity * std::pow(std::max(0.0, b), object->material.p);

        } else {
#ifndef NDEBUG
            printf("should be shadow\n");
#endif
        }
    }
    return color;
}

Vec World::normalize(Vec v) {
    return v/cv::norm(v);
}

Object *World::hit(float &t, Ray& ray, float epsilon) const {
    t = INT_MAX;
    Object* object = nullptr;
    for (auto obj : objects) {
        auto tt = obj->intersect(ray);
        if (tt > epsilon && tt < t) {
            t = tt;
            object = obj;
        }
    }
    if (object) {
        printf("hit ");
        object->repr();
    }
    return object;
}

void World::printVec(Vec &v) {
    printf("(%.2f %.2f %.2f)\n", v[0], v[1], v[2]);
}
