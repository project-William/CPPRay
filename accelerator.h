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

/*
    THIS WAS JUST A TEST
    HAS TO BE REWRITTEN COMPLETELY FROM SCRATCH
    I'LL DO IT SOME BORING DAY
*/

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
        bounds[0] = minv;
        bounds[1] = maxv;
    }

    bool intersect(const Ray &ray, float tmin, float tmax) const
    {
        float tymin, tymax, tzmin, tzmax;
        vec3 ro = ray.getOrigin();
        vec3 rd = ray.getDirection();
        vec3 rd_inv = ray.getDirectionInv();

        tmin = (bounds[ray.sign[0]].x - ro.x) * rd_inv.x;
        tmax = (bounds[1 - ray.sign[0]].x - ro.x) * rd_inv.x;

        tymin = (bounds[ray.sign[1]].y - ro.y) * rd_inv.y;
        tymax = (bounds[1 - ray.sign[1]].y - ro.y) * rd_inv.y;

        if ((tmin > tymax) || (tymin > tmax))
            return false;
        if (tymin > tmin)
            tmin = tymin;
        if (tymax < tmax)
            tmax = tymax;

        tzmin = (bounds[ray.sign[2]].z - ro.z) * rd_inv.z;
        tzmax = (bounds[1 - ray.sign[2]].z - ro.z) * rd_inv.z;

        if ((tmin > tzmax) || (tzmin > tmax))
            return false;
        if (tzmin > tmin)
            tmin = tzmin;
        if (tzmax < tmax)
            tmax = tzmax;

        return true;
    }

    vec3 bounds[2];
    float volume;
};

struct KDPlane
{
    KDPlane(vec3 position, vec3 normal) : m_position(position), m_normal(normal) { }

    bool intersect(const Ray &r, float &t, float tmin, float tmax) const
    {
        vec3 P;
        float d;

        P = m_position - r.getOrigin();
        d = vec3::dot(m_normal, r.getDirection());

        if (d < -EPSILON || d > EPSILON)
            return false;

        t = vec3::dot(P, m_normal) / d;

        if (t < EPSILON)
            return false;

        return true;
    }

    vec3 m_position;
    vec3 m_normal;
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
        std::cout << "KDTree: Root min: " << m_root.box.bounds[0].toString() << " Root max: " << m_root.box.bounds[1].toString() << std::endl;
    }

    void intersect(const Ray &ray, Intersection &xFinal)
    {
        vec3 ro = ray.getOrigin();
        vec3 rd = ray.getDirection();

        float t1, t2;

        KDPlane plane1(m_root.box.bounds[0] - ro, rd);
        KDPlane plane2(m_root.box.bounds[1] - ro, rd);
        plane1.intersect(ray, t1, 0.0f, 1028.0f);
        plane2.intersect(ray, t2, 0.0f, 1028.0f);

        float tmin = (t1 < t2) ? t1 : t2;
        float tmax = (t1 < t2) ? t2 : t1;

        //std::cout << tmin << "    " << tmax << std::endl;

        traverse(&m_root, ray, xFinal, tmin, tmax);
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
        float minx = minmaxlist.front().getCentroid().x;
        float maxx = minmaxlist.back().getCentroid().x;

        std::sort(minmaxlist.begin(), minmaxlist.end(), sort_by_y());
        float miny = minmaxlist.front().getCentroid().y;
        float maxy = minmaxlist.back().getCentroid().y;

        std::sort(minmaxlist.begin(), minmaxlist.end(), sort_by_z());
        float minz = minmaxlist.front().getCentroid().z;
        float maxz = minmaxlist.back().getCentroid().z;

        vec3 minv = vec3(minx, miny, minz);
        vec3 maxv = vec3(maxx, maxy, maxz);

        node->box = KDBox(minv, maxv);
        node->box.volume = (maxx - minx) * (maxy - miny) * (maxz - minz);

        std::cout << "KDTree: Node volume: " << node->box.volume << std::endl;

        // Find the median
        if (axis == 0)
            std::sort(triangles.begin(), triangles.end(), sort_by_x());
        else if (axis == 1)
            std::sort(triangles.begin(), triangles.end(), sort_by_y());
        else
            std::sort(triangles.begin(), triangles.end(), sort_by_z());

        node->median = triangles[median].getCentroid();

        // Create a leaf
        if (triangles.size() <= KDTREE_MIN_TRIS)
        {
            node->data.insert(node->data.end(), triangles.begin(), triangles.end());

            //std::cout << "KDTree: Created a leaf size of " << triangles.size() << " D: " << node->depth << " A: " << axis << " N: " << node->median.toString() << std::endl;
            return;
        }

        // Create branches
        node->left = new KDNode(depth + 1);
        node->right = new KDNode(depth + 1);

        std::vector<Triangle> leftlist(median);
        std::vector<Triangle> rightlist(triangles.size() - median);
        std::copy(triangles.begin(), triangles.begin() + median, leftlist.begin());
        std::copy(triangles.begin() + median, triangles.end(), rightlist.begin());

        //std::cout << "KDTree: Created a branch at " << node->median.toString() << " L: " << leftlist.size() << " R: " << rightlist.size() << " D: " << node->depth << " A: " << axis << std::endl;

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
        const float t = (median - ro).length();

        if (node->isLeaf())
        {
            for (auto &triangle : node->data)
            {
                auto xInit = triangle.intersect(ray);

                if (xInit != invalidIntersection && xInit.getT() < tmax)
                {
                    xFinal = xInit;
                    tmax = xFinal.getT();
                    return;
                }
            }
        }
        else
        {
            traverse(node->left, ray, xFinal, tmin, tmax);
            traverse(node->right, ray, xFinal, tmin, tmax);

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
