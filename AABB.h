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

    }

    int longestAxis() {
        return 0;
    }
};


#endif //RAYTRACING_AABB_H
