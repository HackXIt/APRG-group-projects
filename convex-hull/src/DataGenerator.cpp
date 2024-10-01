//
// Created by RINI on 01/10/2024.
//

#include "DataGenerator.h"

std::vector<ei::Vec2> DataGenerator::GeneratePoints(Generator generator, size_t numPoints)
{
    switch (generator)
    {
        case RANDOM_POINTS:
            return GenerateRandomPoints(numPoints);
        case STRAIGHT_LINE:
            return GenerateStraightLine(numPoints);
        case CIRCLE:
            return GenerateCircle(numPoints);
        case SQUARE:
            return GeneratePointsInSquare(numPoints);
        default:
            throw std::invalid_argument("Invalid generator type.");
    }
}

// 1. Generate a random distribution of points.
std::vector<ei::Vec2> DataGenerator::GenerateRandomPoints(size_t numPoints) {
    std::vector<ei::Vec2> points;
    points.reserve(numPoints);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> x_dist(0.0f, static_cast<float>(WINDOW_DEFAULT_WIDTH));
    std::uniform_real_distribution<float> y_dist(0.0f, static_cast<float>(WINDOW_DEFAULT_HEIGHT));

    for (size_t i = 0; i < numPoints; ++i) {
        float x = x_dist(gen);
        float y = y_dist(gen);
        points.emplace_back(x, y);
    }

    return points;
}

// 2. Generate points forming a straight line.
std::vector<ei::Vec2> DataGenerator::GenerateStraightLine(size_t numPoints) {
    std::vector<ei::Vec2> points;
    points.reserve(numPoints);

    float x0 = 100.0f;
    float y0 = 100.0f;
    float x1 = WINDOW_DEFAULT_WIDTH - 100.0f;
    float y1 = WINDOW_DEFAULT_HEIGHT - 100.0f;

    for (size_t i = 0; i < numPoints; ++i) {
        float t = static_cast<float>(i) / (numPoints - 1);
        float x = x0 + t * (x1 - x0);
        float y = y0 + t * (y1 - y0);
        points.emplace_back(x, y);
    }

    return points;
}

// 3. Generate points forming a circle.
std::vector<ei::Vec2> DataGenerator::GenerateCircle(size_t numPoints) {
    std::vector<ei::Vec2> points;
    points.reserve(numPoints);

    float cx = WINDOW_DEFAULT_WIDTH / 2.0f;
    float cy = WINDOW_DEFAULT_HEIGHT / 2.0f;
    float radius = std::min(WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT) / 2.0f - 50.0f;

    for (size_t i = 0; i < numPoints; ++i) {
        float angle = 2.0f * static_cast<float>(M_PI) * i / numPoints;
        float x = cx + radius * std::cos(angle);
        float y = cy + radius * std::sin(angle);
        points.emplace_back(x, y);
    }

    return points;
}

// 4. Generate a random distribution inside a square box.
std::vector<ei::Vec2> DataGenerator::GeneratePointsInSquare(size_t numPoints) {
    std::vector<ei::Vec2> points;
    points.reserve(numPoints);

    float square_size = std::min(WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT) - 200.0f; // Leave margin
    float x0 = (WINDOW_DEFAULT_WIDTH - square_size) / 2.0f;
    float y0 = (WINDOW_DEFAULT_HEIGHT - square_size) / 2.0f;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0.0f, square_size);

    for (size_t i = 0; i < numPoints; ++i) {
        float x = x0 + dist(gen);
        float y = y0 + dist(gen);
        points.emplace_back(x, y);
    }

    return points;
}
