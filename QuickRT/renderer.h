#pragma once

#ifndef QUICKRT_RENDERER_H
#define QUICKRT_RANDERER_H

#include "scene.h"

class Renderer {
public:

    struct RenderTask {
        int p;
        double cx, cy;
        Color color;
    };

    int render(const Scene& scene, cv::Mat& canvas);

    // perform gamma correlation to picture
    void gamma_corr(cv::Mat& image, double gamma)const;

    // render one sample
    void render_sample(const Scene& scene, RenderTask& task);

    // render one ray
    int render_ray(const Scene& scene, const Ray& ray, Color& color, int reflect_times = 0);

    // trace the hitpoint of one ray
    const Object* trace_ray(const Scene& scene, const Ray& ray, Vec3& hitpoint)const;

    bool has_reflect;
    bool has_transparent;
    bool is_direct;
    double gamma;

    int reflect_limit;
    int anti_alias;

    const double bias = 0.001;

private:

    Rect3 screen_cache;
};

#endif