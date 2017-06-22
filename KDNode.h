//
// Created by 沛然 姚 on 2017/6/22.
//

#ifndef RAYTRACING_KDNODE_H
#define RAYTRACING_KDNODE_H

#include "AABB.h"
#include "Objects.h"

class KDNode {
public:
    AABB bbox;
    KDNode* left;
    KDNode* right;
    std::vector<Triangle *> triangles;

    KDNode(std::vector<Triangle *> & tris);

    static KDNode* build(std::vector<Triangle *>& tris, int depth) ;
    bool hit(const Ray& ray, double& t, Triangle*& tri) const;
};


#endif //RAYTRACING_KDNODE_H
