#pragma once

#include "scene.h"

class Plane : public Object {
public:

    Plane() {

    }

    Plane(const Vec3& center, const Vec3& normal, mtr_t material) :
        Object(center, material),
        normal_cache(cv::normalize(normal)){
    }

    bool hit_by(const Ray& ray, double& hitlen) const {
        hitlen = (pos - ray.start()).dot(normal_cache) / normal_cache.dot(ray.direction());
        return hitlen > 0;
    }
    Vec3 normal(const Vec3& point) const {
        return normal_cache;
    }

    Vec3 normal_cache;
};