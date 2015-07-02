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

struct KDBox
{
    KDBox(vec3 minv = vec3(), vec3 maxv = vec3())
    {
        lb = minv;
        rt = maxv;
    }

    bool intersect(const Ray &ray, float tmin, float tmax) const
    {
        vec3 ro = ray.getOrigin();
        vec3 rd = ray.getDirection();
        vec3 rd_inv = ray.getDirectionInv();

        float t1 = (lb.x - ro.x) * rd_inv.x;
        float t2 = (rt.x - ro.x) * rd_inv.x;
        float t3 = (lb.y - ro.y) * rd_inv.y;
        float t4 = (rt.y - ro.y) * rd_inv.y;
        float t5 = (lb.z - ro.z) * rd_inv.z;
        float t6 = (rt.z - ro.z) * rd_inv.z;

        float tboxmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
        float tboxmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

        if (tboxmax < 0.0f)
        {
            return false;
        }

        if (tboxmin > tboxmax)
        {
            return false;
        }

        tmin = tboxmin;
        tmax = tboxmax;
        return true;
    }

    vec3 getCentroid() const
    {
        vec3 lbrt = rt - lb;
        vec3 lbrt_unitv = lbrt.normalize();
        return lbrt_unitv * (lbrt.length() / 2.0f);
    }

    vec3 lb, rt;
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
    KDNode *left, *right;
    KDBox box;
    vec3 median;
    std::vector<Triangle> data;
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
        std::cout << "KDTree: Face count: " << triangles.size() << std::endl;
    }

    void intersect(const Ray &ray, Intersection &xFinal)
    {
        vec3 ro = ray.getOrigin();
        vec3 rd = ray.getDirection();
        traverse(&m_root, ray, xFinal, 0.0f, MAXDISTANCE);
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
        std::sort(triangles.begin(), triangles.end(), sort_by_x());
        float minx = triangles.front().getMinimum().x;
        float maxx = triangles.back().getMaximum().x;

        std::sort(triangles.begin(), triangles.end(), sort_by_y());
        float miny = triangles.front().getMinimum().y;
        float maxy = triangles.back().getMaximum().y;

        std::sort(triangles.begin(), triangles.end(), sort_by_z());
        float minz = triangles.front().getMinimum().z;
        float maxz = triangles.back().getMaximum().z;

        vec3 minv = vec3(minx, miny, minz);
        vec3 maxv = vec3(maxx, maxy, maxz);

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

        // Add the box and median to node
        node->box = KDBox(minv, maxv);
        node->median = (maxv + minv) / triangles.size();

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

    void traverse(KDNode *node, const Ray &ray, Intersection &xFinal, float tmin, float tmax)
    {
        const unsigned int depth = node->depth;
        const unsigned int axis = depth % m_k;
        const vec3 median = node->median;
        const vec3 ro = ray.getOrigin();
        const vec3 rd = ray.getDirection();

        if (node->isLeaf())
        {
            /*
            node->box.intersect(ray, tmin, tmax);
            vec3 x = ray.getOrigin() + ray.getDirection() * tmin;
            vec3 n = (x - node->box.getCentroid()).normalize().negate();
            xFinal = Intersection(x, n, tmin, Material(DIFF, vec3(), vec3(1, 1, 1)));
            return;
            */

            for (auto &triangle : node->data)
            {
                auto xInit = triangle.intersect(ray);

                if (xInit != invalidIntersection && xInit.getT() > tmin && xInit.getT() < tmax)
                {
                    xFinal = xInit;
                    return;
                }
            }
        }
        else
        {
            if (node->box.intersect(ray, tmin, tmax))
            {
                traverse(node->left, ray, xFinal, tmin, tmax);
                traverse(node->right, ray, xFinal, tmin, tmax);
            }
        }
    }

    unsigned int m_k;
    bool m_initialized;
    KDNode m_root;
protected:
};

}

#endif // ACCELERATOR_H
