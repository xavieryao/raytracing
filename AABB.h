//
// Created by 沛然 姚 on 2017/6/22.
//

#ifndef RAYTRACING_AABB_H
#define RAYTRACING_AABB_H

#include "Ray.h"

/*
 * axis aligned bounding box
 */
struct AABB {
    cv::Vec3d bounds[2];

    AABB() {}

    AABB(cv::Vec3d min, cv::Vec3d max) {
        this->bounds[0] = min;
        this->bounds[1] = max;
    }

    bool intersect(const Ray &r) const
    {
        double tmin, tmax, tymin, tymax, tzmin, tzmax;

        tmin = (bounds[r.sign[0]][0] - r.origin[0]) * r.invDir[0];
        tmax = (bounds[1-r.sign[0]][0] - r.origin[0]) * r.invDir[0];
        tymin = (bounds[r.sign[1]][1] - r.origin[1]) * r.invDir[1];
        tymax = (bounds[1-r.sign[1]][1] - r.origin[1]) * r.invDir[1];

        if ((tmin > tymax) || (tymin > tmax))
            return false;
        if (tymin > tmin)
            tmin = tymin;
        if (tymax < tmax)
            tmax = tymax;

        tzmin = (bounds[r.sign[2]][2] - r.origin[2]) * r.invDir[2];
        tzmax = (bounds[1-r.sign[2]][2] - r.origin[2]) * r.invDir[2];

        if ((tmin > tzmax) || (tzmin > tmax))
            return false;
        if (tzmin > tmin)
            tmin = tzmin;
        if (tzmax < tmax)
            tmax = tzmax;

        return true;
    }

    void expand(AABB& box) {
        for (int i = 0; i < 3; ++i) {
            if (box.bounds[0][i] < bounds[0][i]) bounds[0][i] = box.bounds[0][i];
            if (box.bounds[1][i] > bounds[1][i]) bounds[1][i] = box.bounds[1][i];
        }
    }

    int longestAxis() {
        double ax0 = std::abs(bounds[1][0] - bounds[0][0]);
        double ax1 = std::abs(bounds[1][1] - bounds[0][1]);
        double ax2 = std::abs(bounds[1][2] - bounds[0][2]);

        if (ax0 >= ax1 && ax0 >= ax2) return 0;
        if (ax1 >= ax2 && ax1 >= ax0) return 1;
        else return 2;
    }
};


#endif //RAYTRACING_AABB_H
