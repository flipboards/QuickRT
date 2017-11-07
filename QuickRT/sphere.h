#pragma once

#include "scene.h"

class Sphere : public Object {
public:

    double radius;

    Sphere() {

    }

    Sphere(double radius, const Vec3& center, mtr_t material) : 
        Object(center, material),
        radius(radius) {

    }

    bool hit_by(const Ray& ray, double& hitlen) const {

        Vec3 oc = ray.start() - pos;

        double a = ray.direction().dot(ray.direction());
        double b = ray.direction().dot(oc);
        double c = oc.dot(oc) - radius * radius;

        double delta = b * b - a * c;
        if (delta > 0) {
            hitlen = -(b + sqrt(delta)) / a;
            return hitlen > 0;
        }
        else {
            return false;
        }
    }
    Vec3 normal(const Vec3& point) const {
        return (point - pos) / radius;
    }

};