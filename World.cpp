//
// Created by 沛然 姚 on 2017/4/26.
//

#include "World.h"
#include "tqdm/tqdm.h"

void World::addObject(Object *obj) {
    objects.push_back(obj);
}

void World::render(double l, double r, double b, double t, double d, int nx, int ny, Camera& cam, cv::Mat &mat) {
    auto& uu = cam.u;
    auto& vv = cam.v;
    auto& ww = cam.w;

    for (int i = 0; i < ny; ++i) {
//        printf("column %d\n", i);
        for (int j = 0; j < nx; ++j) {
            verbose =  (i == 46 and (j == 51 or j == 52));
            if (verbose) printf("pixel %d %d\n", i, j);

            auto u = l + (r - l) * (i + 0.5) / ny;
            auto v = b + (t - b) * (j + 0.5) / nx;
            auto direction = -d * ww + u * uu + v * vv;
            auto ray = Ray(cam.eye, direction);
            mat.at<Color>(j, i) = rayTracing(ray);
        }
    }
}

/*
 * l,r,b,t: describes view (how broad you can see)
 * d: view depth
 * nx, ny: height and width
 */
void World::render(double l, double r, double b, double t, double d, int nx, int ny, Camera& cam, bool ssaa) {
    if (!ssaa) {
        auto image = cv::Mat(nx, ny, CV_8UC3, cv::Scalar(bgColor[0], bgColor[1], bgColor[2]));
        render(l, r, b, t, d, nx, ny, cam, image);
        auto filename = "/Users/xavieryao/cg/rt/" + name + ".png";
        cv::imwrite(filename, image);
    } else {
        // super sampling
        auto sampledImage = cv::Mat(nx, ny, CV_8UC3, cv::Scalar(bgColor[0], bgColor[1], bgColor[2]));
        auto image = cv::Mat(nx * 2, ny * 2, CV_8UC3, cv::Scalar(bgColor[0], bgColor[1], bgColor[2]));
        render(l, r, b, t, d, nx * 2, ny * 2, cam, image);

        for (int i = 0; i < nx; ++i) {
            for (int j = 0; j < ny; ++j) {
                sampledImage.at<Color>(i, j) = superSample(i, j, nx, ny, image);
            }
        }
        auto filename = "/Users/xavieryao/cg/rt/" + name + ".png";
        cv::imwrite(filename, sampledImage);
    }

}

Color World::superSample(int i, int j, int nx, int ny, cv::Mat &image)  {
    #define VALID_IDX(x, y) (x > 0 && x < 2*nx && y > 0 && y < 2*ny)
    // 3*3 kernel
    Color mean = 0;
    int dxs[] = {-1, 0, 1};
    int dys[] = {-1, 0, 1};
    double weights[] = {1, 2, 1, 2, 4, 2, 1, 2, 1};
    int pixels = 16;
    int k = 0;
    for (auto dx : dxs) {
        for (auto dy: dys) {
            mean += VALID_IDX(2 * i + dx, 2 * j + dy) ? image.at<Color>(2 *i +dx, 2 * j + dy) * (weights[k]/pixels) :
                    image.at<Color>(2 * i, 2 * j) * (weights[k]/pixels);
            k++;
        }
    }
    return mean;
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
}

Color World::rayTracing(Ray &ray, int depth, double epsilon)  {
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

        Vec lt = light->position - intersection;
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
        Vec r = ray.direction - 2*(n.ddot(ray.direction))*n;
        Ray mirrorRay(intersection, r);
        Color rColor = rayTracing(mirrorRay, depth-1, 0.01);
        color += object->material.km*rColor;
    }
    return color;
}

Vec World::normalize(Vec v) {
    return v / cv::norm(v);
}

Object *World::hit(double &t, Ray &ray, double epsilon, double max)  {
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

bool World::refract(Vec &d, Vec &n, double nt, Vec &t)  {
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
