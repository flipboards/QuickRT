#pragma once

#ifndef QUICKRT_TYPES_H
#define QUICKRT_TYPES_H

#include "opencv2/opencv.hpp"

using Vec3 = cv::Vec3d;
using Scalar = cv::Scalar;
using Rect = cv::Rect2f;

struct Rect3 {
    Vec3 lt, horizon, vertical;

    Vec3 locate(double cx, double cy)const {
        return lt + cx * horizon + cy * vertical;
    }
};

class Ray {
public:

    Ray() {}
    Ray(const Vec3& start, const Vec3& direction) : _start(start), _direction(cv::normalize(direction)) {

    }
    const Vec3& start()const {
        return _start;
    }
    const Vec3& direction()const {
        return _direction;
    }
    Vec3 at(double t)const {
        return _start + _direction * t;
    }

private:
    Vec3 _start, _direction;
};

#endif