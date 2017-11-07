#pragma once

#ifndef QUICKRT_RANDOM_H
#define QUICKRT_RANDOM_H

#include <vector>
#include <random>

void blue_noise_batch(int number, std::vector<double>& x, std::vector<double>& y, double width=1.0, double height=1.0) {

    static std::uniform_real_distribution<double> dis(-0.5  / (number + 1), 0.5 / (number + 1));
    static std::default_random_engine engine;

    for(int i = 0; i < number; i++)
        for (int j = 0; j < number; j++) {
            x[i * number + j] = ((double)(i + 1) / (number + 1) + dis(engine) - 0.5) * width;
            y[i * number + j] = ((double)(j + 1) / (number + 1) + dis(engine) - 0.5) * height;
    }
}

#endif