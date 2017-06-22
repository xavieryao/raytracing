//
// Created by 沛然 姚 on 2017/6/22.
//

#include "KDNode.h"

KDNode::KDNode(std::vector<Triangle *> & tris) {
    this->triangles = tris;
    this->left = nullptr;
    this->right = nullptr;
    this->bbox = AABB();
}

KDNode *KDNode::build(std::vector<Triangle *> &tris, int depth) {
    KDNode* node = new KDNode(tris);

    if (tris.size() == 0) {
        return node;
    }

    if (tris.size() == 1) {
        node->bbox = tris[0]->aabb;
        auto t1 = std::vector<Triangle*>();
        auto t2 = std::vector<Triangle*>();

        node->left = new KDNode(t1);
        node->right = new KDNode(t2);
        return node;
    }

    node->bbox = tris[0]->aabb;
    for (int i = 1; i < tris.size(); i++) {
        node->bbox.expand(tris[i]->aabb);
    }

    cv::Vec3d midpt(0, 0, 0);
    for (int i = 0; i < tris.size(); i++) {
        midpt += tris[i]->getMidPoint() * (1.0/tris.size());
    }

    std::vector<Triangle*> leftTris;
    std::vector<Triangle*> rightTris;
    int axis = node->bbox.longestAxis();
    for (int i = 0; i < tris.size(); ++i) {
        if (midpt[axis] >= tris[i]->getMidPoint()[axis]) {
            rightTris.push_back(tris[i]);
        } else {
            leftTris.push_back(tris[i]);
        }
    }

    if (leftTris.size() == 0 && rightTris.size() > 0) leftTris = rightTris;
    if (rightTris.size() == 0 && leftTris.size() > 0) rightTris = leftTris;

    int matches = 0;
    for (int i = 0; i < leftTris.size(); i++) {
        for (int j = 0; j < rightTris.size(); j++) {
            if (leftTris[i] == rightTris[j]) {
                matches ++;
            }
        }
    }

    if ((float) matches / leftTris.size() < 0.5 && (float) matches / rightTris.size() < 0.5) {
        node->left = build(leftTris, depth + 1);
        node->right = build(rightTris, depth + 1);
    } else {
        auto t1 = std::vector<Triangle*>();
        auto t2 = std::vector<Triangle*>();
        node->left = new KDNode(t1);
        node->right = new KDNode(t2);
    }
    return node;
}

bool KDNode::hit(const Ray &ray, double &t, Triangle*& tri) const {
    t = -3;
    if (!bbox.intersect(ray)) {
        tri = nullptr;
        t = -2;
        return false;
    }
    if (left->triangles.size() > 0 || right->triangles.size() > 0) {
        double tl = INT_MAX, tr = INT_MAX;
        Triangle* tril, *trir;
        bool hitLeft = left->hit(ray, tl, tril);
        if (hitLeft) {
            t = tl;
            tri = tril;
        } else {
            tl = INT_MAX;
        }
        bool hitRight = right->hit(ray, tr, trir);
        if (hitRight) {
            if (tr < tl) {
                t = tr;
                tri = trir;
            }
        }

        return hitLeft || hitRight;
    } else {
        tri = nullptr;
        double min_t = INT_MAX;
        for (auto triangle: triangles) {
            double tt = triangle->intersect(ray);
            if (tt > 0.0000001 && tt > 0 && tt < min_t) {
                tri = triangle;
                min_t = tt;
            }
        }
        if (min_t > 0 && tri != nullptr) {
            t = min_t;
            return true;
        } else {
            return false;
        }
    }
}
