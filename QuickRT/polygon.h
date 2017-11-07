#pragma once

#include "scene.h"

class Triangle : public Object {
public:
    Triangle() {

    }

    Triangle(const Vec3& a, const Vec3& b, const Vec3& c, mtr_t material) :
        Object(a, material),
        normal_cache((b - a).cross(c - a)),
        b_cache(b - a),
        c_cache(c - a){
    }

    bool hit_by(const Ray& ray, double& hitlen) const {
        hitlen = (pos - ray.start()).dot(normal_cache) / normal_cache.dot(ray.direction());
        if (hitlen > 0) {
            Vec3 local_point = ray.at(hitlen) - pos;
            return local_point.cross(b_cache).dot(normal_cache) <= 0 &&
                local_point.cross(c_cache).dot(normal_cache) >= 0 &&
                (local_point - b_cache).cross(c_cache - b_cache).dot(normal_cache) <= 0;
        }
        else {
            return false;
        }
    }
    Vec3 normal(const Vec3& point) const {
        return normal_cache;
    }

    Vec3 normal_cache;
    Vec3 b_cache;
    Vec3 c_cache;
};

class Quad : public Object {
public:
    Quad() {

    }

    Quad(const Vec3& a, const Vec3& b, const Vec3& c, mtr_t material) :
        Object(a, material),
        normal_cache(cv::normalize((b - a).cross(c - a))),
        b_cache(b - a),
        c_cache(c - a) {
    }

    bool hit_by(const Ray& ray, double& hitlen) const {
        hitlen = (pos - ray.start()).dot(normal_cache) / normal_cache.dot(ray.direction());
        if (hitlen > 0) {
            Vec3 local_point = ray.at(hitlen) - pos;
            double pdb = local_point.dot(b_cache),
                pdc = local_point.dot(c_cache);
            return pdb >= 0 && pdb <= b_cache.dot(b_cache) &&
                pdc >= 0 && pdc <= c_cache.dot(c_cache);
        }
        else {
            return false;
        }
    }
    Vec3 normal(const Vec3& point) const {
        return normal_cache;
    }

    Vec3 normal_cache;
    Vec3 b_cache;
    Vec3 c_cache;
};