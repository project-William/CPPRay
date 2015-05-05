#ifndef ACCELERATOR_H
#define ACCELERATOR_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include "config.h"
#include "vec3.h"
#include "triangle.h"

namespace accelerator
{

using namespace math;

struct sort_by_x
{
    inline bool operator() (Triangle &t1, Triangle &t2)
    {
        return (t1.getCentroid().x < t2.getCentroid().x);
    }
};

struct sort_by_y
{
    inline bool operator() (Triangle &t1, Triangle &t2)
    {
        return (t1.getCentroid().y < t2.getCentroid().y);
    }
};

struct sort_by_z
{
    inline bool operator() (Triangle &t1, Triangle &t2)
    {
        return (t1.getCentroid().z < t2.getCentroid().z);
    }
};

struct KDNode
{
    KDNode(unsigned int depth) : depth(depth), left(nullptr), right(nullptr) { }

    ~KDNode()
    {
        if (left != nullptr)
            delete left;
        if (right != nullptr)
            delete right;
    }

    bool isLeaf()
    {
        return (left == nullptr) && (right == nullptr);
    }

    unsigned int depth;
    vec3 median;
    KDNode *left, *right;
    std::vector<Triangle> data;
};

class KDTree
{
public:
    KDTree(unsigned int k = 3) : m_k(k), m_root(0) { }

    void init(std::vector<Triangle> &triangles)
    {
        std::cout << "KDTree: Building... Size: " << triangles.size() << std::endl;

        std::clock_t startTime = std::clock();
        build(&m_root, triangles);
        std::clock_t endTime = std::clock();

        std::clock_t time = endTime - startTime;
        float duration = time / (double) CLOCKS_PER_SEC;

        std::cout << "KDTree: Build finished! Time taken: " << duration << "s." << std::endl;
    }
private:
    void build(KDNode *node, std::vector<Triangle> triangles)
    {
        const unsigned int depth = node->depth;
        const unsigned int axis = depth % m_k;
        const unsigned int median = static_cast<unsigned int>(triangles.size() >> 1);

        if (axis == 0)
            std::sort(triangles.begin(), triangles.end(), sort_by_x());
        else if (axis == 1)
            std::sort(triangles.begin(), triangles.end(), sort_by_y());
        else
            std::sort(triangles.begin(), triangles.end(), sort_by_z());

        node->median = triangles[median].getCentroid();

        if (triangles.size() <= KDTREE_MIN_TRIS)
        {
            std::cout << "KDTree: Created a leaf size of " << triangles.size() << " D: " << node->depth << " A: " << axis << " N: " << node->median.toString() << std::endl;
            node->data.insert(node->data.end(), triangles.begin(), triangles.end());
            return;
        }

        node->left = new KDNode(depth + 1);
        node->right = new KDNode(depth + 1);

        std::vector<Triangle> leftlist(median);
        std::vector<Triangle> rightlist(triangles.size() - median);
        std::copy(triangles.begin(), triangles.begin() + median, leftlist.begin());
        std::copy(triangles.begin() + median, triangles.end(), rightlist.begin());

        std::cout << "KDTree: Created a branch at " << node->median.toString() << " L: " << leftlist.size() << " R: " << rightlist.size() << " D: " << node->depth << " A: " << axis << std::endl;

        build(node->left, leftlist);
        build(node->right, rightlist);
    }

    unsigned int m_k;
    KDNode m_root;
protected:
};

}

#endif // ACCELERATOR_H
