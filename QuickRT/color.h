#pragma once

#ifndef QUICKRT_COLOR_H
#define QUICKRT_COLOR_H

#include "types.h"


class Color {
public:

    Color() {

    }

    explicit Color(double i) : data(0, 0, 0, i) {

    }

    Color(double r, double g, double b, double i) :
        data(r, g, b, i) {
    }

    explicit Color(const Vec3& rgb, double i=1.0) :
        data(rgb[0], rgb[1], rgb[2], i) {
    }

    Color& scale(double ratio) {
        data[0] /= ratio;
        data[1] /= ratio;
        data[2] /= ratio;
        data[3] *= ratio;
        return *this;
    }

    // put relative RGB into intensity
    Color& normalize() {

        return scale(data[0] + data[1] + data[2]);
    }

    Vec3 rgb()const {
        return Vec3(data[0], data[1], data[2]);
    }

    double intensity()const {
        return data[3];
    }

    Color& operator*=(double i) {
        data[3] *= i;
        return *this;
    }

private:

    cv::Vec4d data;
};


namespace Colors {
    const Color Dark = { 0.0, 0.0, 0.0, 0.0 };
    const Color White = { 1.0, 1.0, 1.0, 1.0 };
    const Color Red = { 1.0, 0.0, 0.0, 1.0 };
    const Color Green = { 0.0, 1.0, 0.0, 1.0 };
    const Color Blue = { 0.0, 0.0, 1.0, 1.0 };
}
#endif // !QUICKRT_COLOR_H