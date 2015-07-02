#ifndef ACCELERATOR_H
#define ACCELERATOR_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include "config.h"
#include "vec3.h"
#include "ray.h"
#include "intersection.h"
#include "plane.h"
#include "triangle.h"

namespace accelerator
{

using namespace math;

struct sort_by_x
{
    inline bool operator() (Triangle &t1, Triangle &t2)
    {
        return (t1.getMinimum().x < t2.getMinimum().x);
    }
};

struct sort_by_y
{
    inline bool operator() (Triangle &t1, Triangle &t2)
    {
        return (t1.getMinimum().y < t2.getMinimum().y);
    }
};

struct sort_by_z
{
    inline bool operator() (Triangle &t1, Triangle &t2)
    {
        return (t1.getMinimum().z < t2.getMinimum().z);
    }
};

struct KDBox
{
    KDBox(vec3 minv = vec3(), vec3 maxv = vec3())
    {
        lb = minv;
        rt = maxv;
    }

    bool intersect(const Ray &ray, float t) const
    {
        float tmin, tmax;
        vec3 ro = ray.getOrigin();
        vec3 rd = ray.getDirection();
        vec3 rd_inv = ray.getDirectionInv();

        float t1 = (lb.x - ro.x) * rd_inv.x;
        float t2 = (rt.x - ro.x) * rd_inv.x;
        float t3 = (lb.y - ro.y) * rd_inv.y;
        float t4 = (rt.y - ro.y) * rd_inv.y;
        float t5 = (lb.z - ro.z) * rd_inv.z;
        float t6 = (rt.z - ro.z) * rd_inv.z;

        tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
        tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

        if (tmax < 0.0f)
        {
            t = tmax;
            return false;
        }

        if (tmin > tmax)
        {
            t = tmax;
            return false;
        }

        t = tmin;
        return true;
    }

    vec3 lb, rt;
    float volume;
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
    KDBox box;
};

class KDTree
{
public:
    KDTree(unsigned int k = 3) : m_k(k), m_initialized(false), m_root(0) { }

    void init(std::vector<Triangle> &triangles)
    {
        std::cout << "KDTree: Building... Size: " << triangles.size() << std::endl;

        std::clock_t startTime = std::clock();
        build(&m_root, triangles);
        std::clock_t endTime = std::clock();

        std::clock_t time = endTime - startTime;
        float duration = time / (double) CLOCKS_PER_SEC;

        m_initialized = true;

        std::cout << "KDTree: Build finished! Time taken: " << duration << "s." << std::endl;
        std::cout << "KDTree: Root min: " << m_root.box.lb.toString() << " Root max: " << m_root.box.rt.toString() << std::endl;
    }

    void intersect(const Ray &ray, Intersection &xFinal)
    {
        vec3 ro = ray.getOrigin();
        vec3 rd = ray.getDirection();
        traverse(&m_root, ray, xFinal, MAXDISTANCE);
    }

    bool isInitialized()
    {
        return m_initialized;
    }

private:
    void build(KDNode *node, std::vector<Triangle> triangles)
    {
        const unsigned int depth = node->depth;
        const unsigned int axis = depth % m_k;
        const unsigned int median = static_cast<unsigned int>(triangles.size() >> 1);

        // Calculate node bounding box
        std::vector<Triangle> minmaxlist(triangles.size());
        std::copy(triangles.begin(), triangles.end(), minmaxlist.begin());

        std::sort(minmaxlist.begin(), minmaxlist.end(), sort_by_x());
        float minx = minmaxlist.front().getMinimum().x;
        float maxx = minmaxlist.back().getMaximum().x;

        std::sort(minmaxlist.begin(), minmaxlist.end(), sort_by_y());
        float miny = minmaxlist.front().getMinimum().y;
        float maxy = minmaxlist.back().getMaximum().y;

        std::sort(minmaxlist.begin(), minmaxlist.end(), sort_by_z());
        float minz = minmaxlist.front().getMinimum().z;
        float maxz = minmaxlist.back().getMaximum().z;

        vec3 minv = vec3(minx, miny, minz);
        vec3 maxv = vec3(maxx, maxy, maxz);

        node->box = KDBox(minv, maxv);
        node->box.volume = (maxx - minx) * (maxy - miny) * (maxz - minz);

        //std::cout << "KDTree: Node lb: " << node->box.lb.toString() << " rt: " << node->box.rt.toString() << std::endl;

        // Find the median
        if (axis == 0)
        {
            std::sort(triangles.begin(), triangles.end(), sort_by_x());
        }
        else if (axis == 1)
        {
            std::sort(triangles.begin(), triangles.end(), sort_by_y());
        }
        else
        {
            std::sort(triangles.begin(), triangles.end(), sort_by_z());
        }

        node->median = (maxv + minv) / triangles.size();

        std::cout << "KDTree: Node median: " + node->median.toString() << std::endl;

        // Create a leaf
        if (triangles.size() <= KDTREE_MIN_TRIS)
        {
            node->data.insert(node->data.end(), triangles.begin(), triangles.end());
            return;
        }

        // Create branches
        node->left = new KDNode(depth + 1);
        node->right = new KDNode(depth + 1);

        std::vector<Triangle> leftlist(median);
        std::vector<Triangle> rightlist(triangles.size() - median);
        std::copy(triangles.begin(), triangles.begin() + median, leftlist.begin());
        std::copy(triangles.begin() + median, triangles.end(), rightlist.begin());

        build(node->left, leftlist);
        build(node->right, rightlist);
    }

    void traverse(KDNode *node, const Ray &ray, Intersection &xFinal, float t)
    {
        const unsigned int depth = node->depth;
        const unsigned int axis = depth % m_k;
        const vec3 median = node->median;
        const vec3 ro = ray.getOrigin();
        const vec3 rd = ray.getDirection();

        if (node->isLeaf())
        {
            for (auto &triangle : node->data)
            {
                auto xInit = triangle.intersect(ray);

                if (xInit != invalidIntersection && xInit.getT() < t)
                {
                    xFinal = xInit;
                    t = xFinal.getT();
                    return;
                }
            }
        }
        else
        {
            if (node->right->box.intersect(ray, t))
            {
                traverse(node->right, ray, xFinal, t);
            }

            if (node->left->box.intersect(ray, t))
            {
                traverse(node->left, ray, xFinal, t);
            }
        }
    }

    vec3 getAxis(const unsigned int axis)
    {
        if (axis <= 0)
            return vec3(1, 0, 0);
        else if (axis == 1)
            return vec3(0, 1, 0);
        else if (axis >= 2)
            return vec3(0, 0, 1);
        else
            return vec3(1, 0, 0);
    }

    unsigned int m_k;
    bool m_initialized;
    KDNode m_root;
protected:
};

}

#endif // ACCELERATOR_H
