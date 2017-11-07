#pragma once

/* Material, texture */

#ifndef QUICKRT_MATERIAL_H
#define QUICKRT_MATERIAL_H

#include "types.h"
#include "color.h"

class Material {
public:

    Color ambient;
    Color diffusion;
    Color emission;
    Color reflection;
    Color refraction;

    double refraction_index;

    Material() {

    }

    Material(const Material& m) :
        ambient(m.ambient),
        diffusion(m.diffusion),
        emission(m.emission),
        reflection(m.reflection),
        refraction(m.refraction),
        refraction_index(m.refraction_index) {

    }

    Material(const Color& ambient, const Color& diffusion, const Color& emission, const Color& reflection, const Color& refraction, double refraction_idx=1.0) :
        ambient(ambient), 
        diffusion(diffusion), 
        emission(emission), 
        reflection(reflection), 
        refraction(refraction), 
        refraction_index(refraction_idx) {

    }
};

typedef unsigned int mtr_t;

namespace Materials {
    const Material Paper = {
        { 0.0, 0.0, 0.0, 0.0 },
        { 1.0, 1.0, 1.0, 0.5 },
        { 0.0, 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0, 0.0 },
        1.0
    };

    const Material Mirror = {
        { 0.0, 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0, 0.0 },
        { 1.0, 1.0, 1.0, 1.0 },
        { 0.0, 0.0, 0.0, 0.0 },
        1.0
    };

    const Material Glass = {
        { 0.0, 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0, 0.0 },
        { 1.0, 1.0, 1.0, 1.0 },
        { 1.0, 1.0, 1.0, 1.0 },
        1.5
    };
}

#endif