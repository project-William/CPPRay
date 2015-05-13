#ifndef PROJECTION_H
#define PROJECTION_H

#include "vec3.h"
#include "quaternion.h"
#include "math.h"
#include "ray.h"
#include "camera.h"

using namespace math;

class Perspective
{
public:
    static void project(Ray *ray, const Camera *camera, const float width, const float height, const float ar, const float x, const float y)
    {
        // Tent filter for each ray's xy directions
        auto r1 = 2.0f * math::pseudorand(), dx = r1 < 1.0f ? std::sqrt(r1) - 1.0f : 1.0f - std::sqrt(2.0f - r1);
        auto r2 = 2.0f * math::pseudorand(), dy = r2 < 1.0f ? std::sqrt(r2) - 1.0f : 1.0f - std::sqrt(2.0f - r2);

        // Construct the ray's direction vector and aim it towards the virtual screen's pixel
        auto x_norm = (x + dx - width * 0.5f) / width * ar;
        auto y_norm = (height * 0.5f - y + dy) / height;
        auto v_norm = vec3(x_norm, y_norm, -1.0f);

        // Rotate the ray direction based on camera lookAt direction
        quaternion q = camera->getTransform().getRotation();
        quaternion q_inv = q.conjugate();
        quaternion w(0, v_norm.x, v_norm.y, v_norm.z);
        quaternion r = q * w * q_inv;

        // Assign the new parameters to the chosen ray
        ray->setOrigin(camera->getTransform().getPosition());
        ray->setDirection(vec3(r.x, r.y, r.z));
    }
private:
protected:
};

class Orthographic
{
public:
    static void project(Ray *ray, const Camera *camera, const float width, const float height, const float ar, const float x, const float y)
    {
        // Tent filter for each ray's xy locations
        auto r1 = 2.0f * math::pseudorand(), dx = r1 < 1.0f ? std::sqrt(r1) - 1.0f : 1.0f - std::sqrt(2.0f - r1);
        auto r2 = 2.0f * math::pseudorand(), dy = r2 < 1.0f ? std::sqrt(r2) - 1.0f : 1.0f - std::sqrt(2.0f - r2);

        // Construct the ray's origin vector
        auto e_ortho = camera->getTransform().getPosition();
        auto s_ortho = e_ortho + vec3(x + dx - width * 0.5f + 0.5f, height * 0.5f - y + dy + 0.5f, 5.0f) * camera->getZoom();
        auto p_ortho = camera->getTransform().getRotation().getForwardVector().negate();

        // Rotate the ray origin based on camera lookAt direction
        quaternion q = camera->getTransform().getRotation();
        quaternion q_inv = q.conjugate();
        quaternion w(0, s_ortho.x, s_ortho.y, s_ortho.z);
        quaternion r = q * w * q_inv;

        // Assign the new parameters to the chosen ray
        ray->setOrigin(vec3(r.x, r.y, r.z));
        ray->setDirection(p_ortho);
    }
private:
protected:
};

#endif // PROJECTION_H
