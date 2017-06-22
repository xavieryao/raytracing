//
// Created by 沛然 姚 on 2017/6/22.
//

#ifndef RAYTRACING_AABB_H
#define RAYTRACING_AABB_H

#include "Ray.h"

#define NUMDIM	3
#define RIGHT	0
#define LEFT	1
#define MIDDLE	2

/*
 * axis aligned bounding box
 */
struct AABB {
    cv::Vec3d min, max;

    AABB() {}

    AABB(cv::Vec3d min, cv::Vec3d max) {
        this->min = min;
        this->max = max;
    }

    bool intersect(const Ray &r) const
    {
        cv::Vec3d coord;
        bool inside = true;
        char quadrant[NUMDIM];
        int i;
        int whichPlane;
        double maxT[NUMDIM];
        double candidatePlane[NUMDIM];

        /* Find candidate planes; this loop can be avoided if
           rays cast all from the eye(assume perpsective view) */
        for (i=0; i<NUMDIM; i++)
            if(r.origin[i] < min[i]) {
                quadrant[i] = LEFT;
                candidatePlane[i] = min[i];
                inside = false;
            }else if (r.origin[i] > max[i]) {
                quadrant[i] = RIGHT;
                candidatePlane[i] = max[i];
                inside = false;
            }else	{
                quadrant[i] = MIDDLE;
            }

        /* Ray origin inside bounding box */
        if(inside)	{
            return true;
        }


        /* Calculate T distances to candidate planes */
        for (i = 0; i < NUMDIM; i++)
            if (quadrant[i] != MIDDLE && r.direction[i] !=0.)
                maxT[i] = (candidatePlane[i]-r.origin[i]) / r.direction[i];
            else
                maxT[i] = -1.;

        /* Get largest of the maxT's for final choice of intersection */
        whichPlane = 0;
        for (i = 1; i < NUMDIM; i++)
            if (maxT[whichPlane] < maxT[i])
                whichPlane = i;

        /* Check final candidate actually inside box */
        if (maxT[whichPlane] < 0.) return false;
        for (i = 0; i < NUMDIM; i++)
            if (whichPlane != i) {
                coord[i] = r.origin[i] + maxT[whichPlane] * r.direction[i];
                if (coord[i] < min[i] || coord[i] > max[i])
                    return false;
            } else {
                coord[i] = candidatePlane[i];
            }
        return true;				/* ray hits box */
    }

    void expand(AABB& box) {

    }

    int longestAxis() {
        return 0;
    }
};


#endif //RAYTRACING_AABB_H
