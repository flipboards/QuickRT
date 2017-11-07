#pragma once

/* Mesh elements */

#ifndef QUICKRT_MESH_H
#define QUICKRT_MESH_H

#include "types.h"
#include "material.h"

class Vertex {
public:
    Vec3 pos;
    Vec3 normal;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned> indices;  // face indices (3xn or 4xn)
    Material* material;

    enum Type {
        TRIANGLE,
        QUAD
    } type;

};

#endif