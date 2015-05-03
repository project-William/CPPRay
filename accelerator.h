#ifndef ACCELERATOR_H
#define ACCELERATOR_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "vec3.h"

namespace accelerator
{

using namespace math;

struct KDNode
{
    KDNode(unsigned int depth) : depth(depth)
    {
        left = nullptr;
        right = nullptr;
    }

    ~KDNode()
    {
        // Just a placeholder for debugging...
        std::cout << "Deleting a KDNode!" << " Depth: " << depth << " Position: " << position.toString() << std::endl;
        //if (left != nullptr)
        //    delete left;
        //if (right != nullptr)
        //    delete right;
    }

    bool isLeaf()
    {
        return (left == nullptr) | (right == nullptr);
    }

    unsigned int depth;
    vec3 position;
    KDNode *left, *right;
};

class KDTree
{
public:
    KDTree(std::vector<vec3> pointlist = { }, unsigned int k = 3) : m_root(0), m_k(k)
    {
        build(&m_root, pointlist);
    }
private:
    void build(KDNode *node, std::vector<vec3> pointlist)
    {
        if (pointlist.size() <= 1)
            return;

        const unsigned int median = pointlist.size() >> 1;
        const unsigned int depth = node->depth;
        const unsigned int axis = depth % m_k;
        node->position = pointlist[median];
        node->left = new KDNode(depth + 1);
        node->right = new KDNode(depth);

        std::vector<vec3> leftlist(median);
        std::vector<vec3> rightlist(pointlist.size() - median);
        std::copy(pointlist.begin(), pointlist.begin() + median, leftlist.begin());
        std::copy(pointlist.begin() + median, pointlist.end(), rightlist.begin());

        std::cout << "Adding " << leftlist.size() << " entries to left node." << " | Adding " << rightlist.size() << " entries to right node." << std::endl;
        std::cout << "Median: " << median << " | Depth: " << depth << " | Axis: " << axis << " | Node: " << node->position.toString() << std::endl;

        build(node->left, leftlist);
        build(node->right, rightlist);
    }

    unsigned int m_k;
    KDNode m_root;
protected:
};

}

#endif // ACCELERATOR_H
