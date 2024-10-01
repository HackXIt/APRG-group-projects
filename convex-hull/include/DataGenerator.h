//
// Created by RINI on 01/10/2024.
//

#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include "ei/2dtypes.hpp"
#include "app.h"

enum Generator
{
    RANDOM_POINTS = 0,
    STRAIGHT_LINE = 1,
    CIRCLE = 2,
    SQUARE = 3,
    LARGE_DATASET = 4
};

class DataGenerator {
public:
    static std::vector<ei::Vec2> GeneratePoints(Generator generator, size_t numPoints);
private:
    // 1. Generate a random distribution of points.
    static std::vector<ei::Vec2> GenerateRandomPoints(size_t numPoints);

    // 2. Generate points forming a straight line.
    static std::vector<ei::Vec2> GenerateStraightLine(size_t numPoints);

    // 3. Generate points forming a circle.
    static std::vector<ei::Vec2> GenerateCircle(size_t numPoints);

    // 4. Generate a random distribution inside a square box.
    static std::vector<ei::Vec2> GeneratePointsInSquare(size_t numPoints);

    // 5. Generate a large dataset of 100 million data points.
    static std::vector<ei::Vec2> GenerateLargeDataset(size_t numPoints);
};



#endif //DATAGENERATOR_H
