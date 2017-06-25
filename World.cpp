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
//                verbose = (i == 19 && j == 112 && k==1);
//                if (i == 19 && j == 112 && k==1) {
//                    verbose = true;
//                }
                auto u = l + (r - l) * (i + sampleXs[k]) / ny;
                auto v = b + (t - b) * (j + sampleYs[k]) / nx;
                auto direction = -d * ww + u * uu + v * vv;
                auto ray = Ray(cam.randomEye(cameraXs[k], cameraYs[k]), direction);
                Color color = rayTracing(ray);
                if (i == 19 && j == 112 && k==1) {
                    printf("%d %d %d\n", color[0], color[1], color[2]);
                }
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
    if (n.ddot(v) < 0) {
        n = -n;
    }

    verbose = (object->name == "Untitled") && this->name == "4";

    if (verbose) {
        n = -n;
    }

    if (verbose) {
        printf("交点 "); printVec(intersection);
        printf("法线 "); printVec(n);
        printf("视线 "); printVec(v);
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

        constexpr double _ep = 0.0001;

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

        if (verbose) {
            printf("shadow\n");
        }
        double s;
        if (verbose) printf("triangle\n");
        Object* shadowObj = hit(s, shadowRay, 0.01, max);
        if (!shadowObj) {
            Vec h = normalize(v + lt);
            double a = n.ddot(lt);
            double b = n.ddot(h);
//            printf("%f %f\n",a, b);
            color += object->material.color * light->intensity * std::max(0.0, a);
            color += object->material.ks * light->intensity * std::pow(std::max(0.0, b), object->material.p);
        } else if (verbose) {
            printf("shadowed by:\n");
            shadowObj->repr();
        }
        verbose = false;
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
        if (tt > epsilon && tt < t && tt < max) {
            t = tt;
            object = obj;
            if (verbose) {
//                printf("接受 %s\n");
            }
        }
    }

    double tt = -1;
    Triangle* tri = nullptr;

    for (auto kdtree: kdtrees) {
        if (kdtree->hit(ray, tt, tri)) {
            if (tt > epsilon && tt < t && tt < max) {
                t = tt;
                object = tri;
            }
        }
    }

    if (verbose) {
        if (object == nullptr) {
            printf("no hit.\n");
        } else {
            printf("hit %s at %f\n", object->name.c_str(), t);
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

void World::printColor(Vec &c) {
    printf("(%f %f %f)\n", c[0], c[1], c[2]);
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
    for (int i : tqdm::range(ny)) {
        for (int j = 0; j < nx; ++j) {
            std::vector<double> sampleXs(sampleTimes);
            std::vector<double> sampleYs(sampleTimes);
            std::vector<double> cameraXs(sampleTimes);
            std::vector<double> cameraYs(sampleTimes);

            if (verbose) printf("pixel %d %d\n", i, j);

            double blue = 0;
            double green = 0;
            double red = 0;
            /*
             * generate sample points
             */
            for (int ii = 0; ii < sampleTimes; ++ii) {
                sampleXs[ii] = random();
                sampleYs[ii] = random();
                cameraXs[ii] = random();
                cameraYs[ii] = random();
            }
            shuffle(cameraXs);
            shuffle(cameraYs);

            for (int k = 0; k < sampleTimes; k++) {
                verbose = (i == 366 & j == 163 && k == 0);
                auto u = l + (r - l) * (i + sampleXs[k]) / ny;
                auto v = b + (t - b) * (j + sampleYs[k]) / nx;
                auto direction = -d * ww + u * uu + v * vv;
                auto ray = Ray(cam.randomEye(cameraXs[k], cameraYs[k]), direction);
                Vec color = pathTracing(ray);
                blue += color[0] / sampleTimes;
                green += color[1] / sampleTimes;
                red += color[2] / sampleTimes;
            }

            Vec v(blue, green, red);
            mat.at<Color>(j, i) = vec2color(v);
        }
    }

    /*
     * save image
     */
    auto filename = "/Users/xavieryao/cg/pt/" + name + ".png";
    cv::imwrite(filename, mat);
}

Vec World::pathTracing(Ray &ray, int depth, double epsilon) {
    constexpr double _ep = 0.0001;

    double t;
    Object *object = hit(t, ray, epsilon);
    if (object == nullptr) {
        if (verbose) {
            printf("no object hit, return bgcolor\n");
        }
        return color2vec(bgColor);
    }

    if (verbose) {
        printf("hit object %s\n", object->name.c_str());
    }

    Vec intersection = ray.origin + t * ray.direction;
    Vec n = object->normalVector(intersection);
    Vec v = normalize(ray.origin - intersection);
    Vec d = normalize(ray.direction);
    if (n.ddot(v) < 0) {
        n = -n;
    }


    Vec objColor = color2vec(object->material.color);
    double p = std::max(objColor[0], std::max(objColor[1], objColor[2]));

    if (depth > 6) {
        if (std::abs(random()) > p) {
            if (verbose) {
                printf("deptn %d rnd > p\n", depth);
            }
            return object->material.emission;
        } else {
            objColor *= (1/p);
            if (verbose) printf("depth %d rnd <= p = %f, objColor %f %f %f\n", depth, p, objColor[0], objColor[1], objColor[2]);
        }
    }

    /*
    * Refraction
    */

    if (object->material.dielectric) {
        if (verbose) printf("dielectric\n");

        double kr, kg, kb;
        Vec r = d - 2*(n.ddot(d))*n;
        Ray reflectRay(intersection, r);

        if (verbose) {
            printf("反射线");
            printVec(reflectRay.direction);
        }

        Vec tt;
        double c = 0;


        if (d.ddot(n) < 0) { // in
            if (verbose) {
                printf("in\n");
            }
            refract(d, n, object->material.nt, tt);
            c = -d.ddot(n);
            kr = 1; kg = 1; kb = 1;
        } else { // out
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
                Vec color = pathTracing(reflectRay, depth+1, _ep);
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
        Vec reflectColor = pathTracing(reflectRay, depth+1, _ep);
        if (verbose) {printf("reflectColor:");printColor(reflectColor);}
        if (verbose) printf("折射\n");
        Vec refractColor = pathTracing(refractRay, depth+1, _ep);
        if (verbose) {printf("refractColor:");printColor(refractColor);}

        Vec color = R*reflectColor + (1-R)*refractColor;

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

    if (object->material.km != .0) {
        if (verbose) printf("mirror \n");
        Vec r = ray.direction - 2*(n.ddot(ray.direction))*n;
        Ray mirrorRay(intersection, r);
        Vec rColor = pathTracing(mirrorRay, depth+1, _ep);
        if (verbose) printf("mirror %f %f %f", rColor[0], rColor[1], rColor[2]);
        return object->material.km*rColor;
    }

    if (verbose) printf("diffusion\n");
    Vec dir = cosineRandomUnitVec(n);
    Ray rndRay = Ray(intersection, dir);
    Vec diffColor = pathTracing(rndRay, depth+1, _ep);
    if (verbose) printf("diff color %f %f %f\n", diffColor[0], diffColor[1], diffColor[2]);

    double cos_theta = dir.ddot(n) * 2;
    assert(cos_theta >= 0);

    double blue = objColor[0], green = objColor[1], red = objColor[2];
    blue *= diffColor[0] * cos_theta; green *= diffColor[1]*cos_theta; red *= diffColor[2]*cos_theta;


    Vec mixColor(blue, green, red);
    if (verbose) printf("mix color %f %f %f\n", mixColor[0], mixColor[1], mixColor[2]);

    Vec returnColor = object->material.emission + mixColor;
    if (verbose) printf("return color %f %f %f\n", returnColor[0], returnColor[1], returnColor[2]);

    return returnColor;
}

void World::addKDTree(KDNode *node) {
    this->kdtrees.push_back(node);
}

void World::clearObjects() {
    this->objects.clear();
}
