#pragma once

#include "color.h"

enum BlendMode {
    LINEAR, MULTIPLY
};

// blend two lights
Color blend_linear(const Color& lhs, const Color& rhs) {
    Color result(
        lhs.rgb() * lhs.intensity() + rhs.rgb() * rhs.intensity(), 1.0
    );
    result.scale(lhs.intensity() + rhs.intensity());
    return result;
}

// blend light + surface
Color blend_multiply(const Color& lhs, const Color& rhs) {

    Color result(
        lhs.rgb() * rhs.rgb(), 1.0
    );
    result.normalize();
    result *= lhs.intensity() * rhs.intensity();
    return result;
}

Vec3 blend_luminos(const Color& color) {
    return color.intensity() * color.rgb();
}

Color blend(const Color& lhs, const Color& rhs, BlendMode mode) {
    switch (mode) {
    default:
    case LINEAR:
        return blend_linear(lhs, rhs);
    case MULTIPLY:
        return blend_multiply(lhs, rhs);
    }
}

