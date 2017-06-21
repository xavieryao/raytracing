//
// Created by 沛然 姚 on 2017/4/26.
//

#include "World.h"
#include "tqdm/tqdm.h"

void World::addObject(Object *obj) {
    objects.push_back(obj);
}

/*
 * l,r,b,t: describes view (how broad you can see)
 * d: view depth
 * nx, ny: height and width
 */
void World::render(double l, double r, double b, double t, double d, int nx, int ny, Camera& cam, unsigned sampleTimes){
    auto mat = cv::Mat(nx, ny, CV_8UC3, cv::Scalar(bgColor[0], bgColor[1], bgColor[2]));

    auto& uu = cam.u;
    auto& vv = cam.v;
    auto& ww = cam.w;

#pragma omp parallel for collapse(2), schedule(dynamic, 1)
    for (int i = 0; i < ny; ++i) {
        for (int j = 0; j < nx; ++j) {
            std::vector<double> sampleXs(sampleTimes);
            std::vector<double> sampleYs(sampleTimes);
            std::vector<double> cameraXs(sampleTimes);
            std::vector<double> cameraYs(sampleTimes);

            if (verbose) printf("pixel %d %d\n", i, j);

            unsigned long blue = 0;
            unsigned long green = 0;
            unsigned long red = 0;
            /*
             * generate sample points
             */
            for (int i = 0; i < sampleTimes; ++i) {
                sampleXs[i] = random();
                sampleYs[i] = random();
                cameraXs[i] = random();
                cameraYs[i] = random();
            }
            shuffle(cameraXs);
            shuffle(cameraYs);

            #pragma omp parallel for
            for (int k = 0; k < sampleTimes; k++) {
                auto u = l + (r - l) * (i + sampleXs[k]) / ny;
                auto v = b + (t - b) * (j + sampleYs[k]) / nx;
                auto direction = -d * ww + u * uu + v * vv;
                auto ray = Ray(cam.randomEye(cameraXs[k], cameraYs[k]), direction);
                Color color = rayTracing(ray);
                blue += color[0];
                green += color[1];
                red += color[2];
            }

            mat.at<Color>(j, i) = Color(static_cast<uchar>(blue / sampleTimes), static_cast<uchar>(green / sampleTimes),
                                        static_cast<uchar>(red / sampleTimes));
        }
    }

    /*
     * save image
     */
    auto filename = "/Users/xavieryao/cg/rt/" + name + ".png";
    cv::imwrite(filename, mat);
}


void World::addLightSource(Light *l) {
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

std::string World::getName()  {
    return name;
}

World::World(Color bgColor, double aIntensity, std::string name) {
    this->name = name;
    this->aIntensity = aIntensity;
    this->bgColor = bgColor;
    this->verbose = false;
}

Color World::rayTracing(Ray &ray, int depth, double epsilon){
    double t;
    Object *object = hit(t, ray, epsilon);
    if (object == nullptr) {
        return bgColor;
    }

    Vec intersection = ray.origin + t * ray.direction;
    Vec n = object->normalVector(intersection);
    Vec v = normalize(ray.origin - intersection);
    Vec d = normalize(ray.direction);

    if (verbose) {
        printf("交点 "); printVec(intersection);
        printf("法线 "); printVec(n);
        printf("实现 "); printVec(v);
    }

    /*
    * Refraction
    */
    if (object->material.dielectric) {
        if (depth == 0) {
            if (verbose) {
                printf("depth = 0\n");
            }
            return Color(0, 0, 0);
        }

        double kr, kg, kb;
        Vec r = d - 2*(n.ddot(d))*n;
        Ray reflectRay(intersection, r);

        if (verbose) {
            printf("反射线");
            printVec(reflectRay.direction);
        }

        Vec tt;
        double c = 0;

        constexpr double _ep = 0.01;

        if (d.ddot(n) < 0) { /* in */
            if (verbose) {
                printf("in\n");
            }
            refract(d, n, object->material.nt, tt);
            c = -d.ddot(n);
            kr = 1; kg = 1; kb = 1;
        } else { /* out */
            if (verbose) {
                printf("out\n");
            }

            kr = exp(-object->material.ar*std::abs(t));
            kg = exp(-object->material.ag*std::abs(t));
            kb = exp(-object->material.ab*std::abs(t));
            auto neg_n = -n;

            if (refract(d, neg_n, 1/object->material.nt, tt)) {
                c = (normalize(tt)).ddot(n);
            } else {
                Color color = rayTracing(reflectRay, depth-1, _ep);
                color[0] *= kb;
                color[1] *= kg;
                color[2] *= kr;
                if (verbose) {
                    printf("full refraction: "); printColor(color);
                    printf("\n");
                }

                return color;
            }
        }
        Ray refractRay(intersection, tt);
        if (verbose) {
            printf("折射线\n");
            printVec(tt);
        }
        auto R0 = pow((object->material.nt-1)/(object->material.nt+1), 2);
        auto R = R0 + (1-R0)*pow(1-c, 5);

        assert(R >= 0 && R <= 1);

        if (verbose) {
            printf("反射\n");
        }
        Color reflectColor = rayTracing(reflectRay, depth-1, _ep);
        if (verbose) {printf("reflectColor:");printColor(reflectColor);}
        if (verbose) printf("折射\n");
        Color refractColor = rayTracing(refractRay, depth-1, _ep);
        if (verbose) {printf("refractColor:");printColor(refractColor);}

        Color color = R*reflectColor + (1-R)*refractColor;

        if (verbose) printf("kb %.2f kg %.2f kr %.2f   before color: ", kb, kg, kr);
        if (verbose) printColor(color);
        color[0] *= kb;
        color[1] *= kg;
        color[2] *= kr;
        if (verbose) printf("after color: ");
        if (verbose) printColor(color);
        if (verbose) printf("\n");
        return color;
    }


    Color color = object->material.ka * aIntensity;
    for (Light *light: lightSources) {

        Vec lt = light->randomPoint() - intersection;
        double max = cv::norm(lt);
        lt = normalize(lt);
        Ray shadowRay(intersection, lt);

        double s;
        if (!hit(s, shadowRay, 0.01, max)) {
            Vec h = normalize(v + lt);
            double a = n.ddot(lt);
            double b = n.ddot(h);
            color += object->material.color * light->intensity * std::max(0.0, a);
            color += object->material.ks * light->intensity * std::pow(std::max(0.0, b), object->material.p);
        }
    }

    /*
     * ideal specular reflection
     */
    if (depth == 0) {
        return color;
    }
    if (object->material.km != .0) {
//        this->verbose = true;
        Vec r = ray.direction - 2*(n.ddot(ray.direction))*n;
        Ray mirrorRay(intersection, r);
        Color rColor = rayTracing(mirrorRay, depth-1, 0.01);
        color += object->material.km*rColor;
//        this->verbose = false;
    }
    return color;
}

Vec World::normalize(Vec v) {
    return v / cv::norm(v);
}

Object *World::hit(double &t, Ray &ray, double epsilon, double max) {
    t = INT_MAX;
    Object *object = nullptr;
    for (auto obj : objects) {
        auto tt = obj->intersect(ray);
        if (verbose) {
            printf("对物体 %s\n", obj->name.c_str());
            printf("tt=%f\n", tt);
        }
        if (tt > epsilon && tt < t && tt < max) {
            t = tt;
            object = obj;
            if (verbose) {
                printf("接受\n");
            }
        }
    }

    return object;
}

void World::printVec(Vec &v) {
    printf("(%.2f %.2f %.2f)\n", v[0], v[1], v[2]);
}

bool World::refract(Vec &d, Vec &n, double nt, Vec &t){
    /*
     * assuming the refractive index of the environment is 1.
     */

    double a = 1- (1- std::pow(d.ddot(n), 2))/std::pow(nt, 2);
    if (a < 0) {
        return false;
    }
    t = (d-n*d.ddot(n))/nt - n*sqrt(a);
    return true;
}

void World::printColor(Color &c) {
    printf("(%d %d %d)\n", c[0], c[1], c[2]);
}

/*
 * render with brute force path tracing
 */
void World::renderPT(double l, double r, double b, double t, double d, int nx, int ny, Camera& cam, unsigned sampleTimes) {
    auto mat = cv::Mat(nx, ny, CV_8UC3, cv::Scalar(bgColor[0], bgColor[1], bgColor[2]));

    auto& uu = cam.u;
    auto& vv = cam.v;
    auto& ww = cam.w;

#pragma omp parallel for collapse(2), schedule(dynamic, 1)
    for (int i = 0; i < ny; ++i) {
        for (int j = 0; j < nx; ++j) {
//            std::vector<double> sampleXs(sampleTimes);
//            std::vector<double> sampleYs(sampleTimes);
//            std::vector<double> cameraXs(sampleTimes);
//            std::vector<double> cameraYs(sampleTimes);

            if (verbose) printf("pixel %d %d\n", i, j);

            double blue = 0;
            double green = 0;
            double red = 0;

//            shuffle(cameraXs);
//            shuffle(cameraYs);

#pragma omp parallel for
            for (int k = 0; k < sampleTimes; k++) {
                auto u = l + (r - l) * (i + random()) / ny;
                auto v = b + (t - b) * (j + random()) / nx;
                auto direction = -d * ww + u * uu + v * vv;
                auto ray = Ray(cam.eye, direction);
                Vec emission = pathTracing(ray);
                blue += emission[0] / sampleTimes;
                green += emission[1] / sampleTimes;
                red += emission[2] / sampleTimes;
            }

            if (blue < 0.) blue = 0.;
            if (green < 0.) green = 0.;
            if (red < 0.) red = 0.;
            if (blue > 1.) blue = 1.;
            if (green > 1.) green = 1.;
            if (red > 1.) red = 1.;

            Color color = Color(static_cast<uchar>(blue * 255), static_cast<uchar>(green * 255),
                                static_cast<uchar>(red * 255));
            mat.at<Color>(j, i) = color;
        }
    }

    /*
     * save image
     */
    auto filename = "/Users/xavieryao/cg/pt/" + name + ".png";
    cv::imwrite(filename, mat);
}

Vec World::pathTracing(Ray &ray, int depth, double epsilon) {

    double t;
    Object *object = hit(t, ray, epsilon);
    if (object == nullptr) {
        printf("no hit\n");
        return Vec(0, 0, 0);
    }

    Vec intersection = ray.origin + t * ray.direction;
    Vec n = object->normalVector(intersection);
    Vec d = normalize(ray.direction);
    n = n.ddot(ray.direction) < 0 ? n : -n;

    Material& material = object->material;
    Vec f = Vec(material.color[0]/255., material.color[1]/255., material.color[2]/255.);

    double p = std::max(f[0], std::max(f[1], f[2]));
    if (depth > 8) {
        if (std::abs(random()) < p) {
            f = f * (1/p);
        } else {
            return material.emission;
        }
    }

    if (material.dielectric) { // refraction
        double kr, kg, kb;
        Vec r = d - 2*(n.ddot(d))*n;
        Ray reflectRay(intersection, r);

        Vec tt;
        double c = 0;

        constexpr double _ep = 0.01;

        if (d.ddot(n) < 0) { /* in */
            refract(d, n, object->material.nt, tt);
            c = -d.ddot(n);
            kr = 1; kg = 1; kb = 1;
        } else { /* out */
            kr = exp(-object->material.ar*std::abs(t));
            kg = exp(-object->material.ag*std::abs(t));
            kb = exp(-object->material.ab*std::abs(t));
            auto neg_n = -n;

            if (refract(d, neg_n, 1/object->material.nt, tt)) {
                c = (normalize(tt)).ddot(n);
            } else {
                Vec emission = pathTracing(reflectRay, depth+1, _ep);
                emission[0] *= kb;
                emission[1] *= kg;
                emission[2] *= kr;
                return emission;
            }
        }
        Ray refractRay(intersection, tt);
        auto R0 = std::pow((object->material.nt-1)/(object->material.nt+1), 2);
        auto R = R0 + (1-R0)*std::pow(1-c, 5);

        assert(R >= 0 && R <= 1);

        Vec reflectEmission = pathTracing(reflectRay, depth+1, _ep);
        Vec refractEmission = pathTracing(refractRay, depth+1, _ep);

        Vec emission = R*reflectEmission + (1-R)*refractEmission;

        emission[0] *= kb;
        emission[1] *= kg;
        emission[2] *= kr;
        return emission;
    } else if (material.km != 0) { // reflection
        Vec emmittance = material.emission;
        Vec r = d - 2*(n.ddot(d))*n;
        Ray reflectRay(intersection, r);
        return emmittance + pathTracing(reflectRay, depth+1, 0.00001);
    } else { // diffusion
        Vec emmittance = material.emission;
        double r1 = 2*M_PI*static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
        double r2 = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
        double r2s = std::sqrt(r2);

        Vec w = n;
        Vec u = normalize(product((fabs(w[0])>.1?Vec(0,1,0):Vec(1,0,0)),w));
        Vec v = product(w, u);
        Vec dir = normalize(u*std::cos(r1)*r2s + v*std::sin(r1)*r2s + w*std::sqrt(std::max(.0, 1-r2)));

        Ray newRay(intersection, dir);

        double cosTheta = dir.ddot(n);
        Vec BRDF;
        for (int i = 0; i < 3; ++i) {
            BRDF[i] = (1./255.) * 2 * f[i] * cosTheta;
        }
        Vec reflected = pathTracing(newRay, depth+1, 0.0001);
        for (int i = 0; i < 3; ++i) {
            emmittance[i] += BRDF[i] * reflected[i];
        }
        return emmittance;
    }

}