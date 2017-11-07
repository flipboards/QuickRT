#pragma once

/* 3d objects, camera, light */

#ifndef QUICKRT_SCENE_H
#define QUICKRT_SCENE_H

#include "types.h"
#include "material.h"
#include "mesh.h"
#include <vector>

class Moveable {
public:

    Vec3 pos;

    Moveable() {

    }

    Moveable(const Vec3& pos) : pos(pos) {

    }
};


    // 3d object
class Object : public Moveable{
public:
    mtr_t material;

    Object() {

    }

    Object(const Vec3& center, const mtr_t material) :
        Moveable(center),
        material(material)
    {

    }

    virtual void to_mesh(double grid, Mesh& mesh)const {
        fprintf(stderr, "This is root function. Please implement your mesh\n");
    }

    virtual bool hit_by(const Ray& ray, double& t) const {
        fprintf(stderr, "This is root function. Please implement your mesh\n");
        return false;
    }

    // return uniformed normal
    virtual Vec3 normal(const Vec3& point) const{
        fprintf(stderr, "This is root function. Please implement your mesh\n");
        return Vec3();
    }
};


class Light : public Moveable {
public:

    Color color;

    Light() {

    }

    Light(const Vec3& pos, const Color& color) :
        Moveable(pos), color(color) {

    }
};

    // Camera
class Camera : public Moveable {
public:

    Camera() {

    }

    Camera(const Vec3& pos, const Vec3& direction, const Vec3& up = Vec3(0, 1, 0)) :
        Moveable(pos), direction(direction), up(up)
    {

    }

    void set_projection(double width, double aspect, double near, double far) {
        this->width = width;
        this->aspect = aspect;
        this->near = near;
        this->far = far;
    }

    Rect3 get_screen()const {
        Vec3 center = pos + near * direction;
        Vec3 horizon = cv::normalize(direction.cross(up));

        return {
            center - horizon * width / 2 + up * width * aspect / 2,
            horizon * width,
            -up * width * aspect
        };
    }

private:

    Vec3 direction;
    Vec3 up;
    double near;
    double far;
    double width;
    double aspect;

};

    // scene to put all objects
class Scene {
public:

    std::vector<Object*> objects;
    std::vector<Material> materials;

    Color bg_color;
    Color ambient_color;

    Light light;
    Camera camera;

    const Material* get_material(mtr_t material)const {
        return &materials[material];
    }
};

#endif // !QUICKRT_SCENE_H
