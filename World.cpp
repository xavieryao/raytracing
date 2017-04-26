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

    auto uu = Vec(1, 0, 0);
    auto vv = Vec(0, 1, 0);
    auto ww = Vec(0, 0, -1);

    for (int i = 0; i < ny; ++i) {
        printf("column %d\r", i);
        for (int j = 0; j < nx; ++j) {
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
    float t = INT_MAX;
    Object* object = nullptr;
    for (auto obj : objects) {
        auto tt = obj->intersect(ray);
        if (tt > 0 && tt < t) {
            t = tt;
            object = obj;
        }
    }
    if (object == nullptr) {
        return bgColor;
    }
    Vec intersection = ray.origin + t*ray.direction;
    Vec n = object->normalVector(intersection);
    Vec v = normalize(ray.origin - intersection);

    Color color = object->ka * aIntensity;
    for (Light* light: lightSources) {
        Vec lt = normalize(light->position - intersection);
        Vec h = normalize(v + lt);
        color += object->color * light->intensity * std::max(0.0, n.ddot(lt));
        color += object->ks * light->intensity * std::pow(std::max(0.0, n.ddot(h)), object->p);
    }
    return color;
}

Vec World::normalize(Vec v) {
    return v/cv::norm(v);
}
