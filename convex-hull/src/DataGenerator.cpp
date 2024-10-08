﻿//
// Created by RINI on 01/10/2024.
//

#include "DataGenerator.h"

#include <iostream>

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
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <ei/2dtypes.hpp>

std::vector<ei::Vec2> DataGenerator::GenerateStraightLine(size_t numPoints) {
    std::vector<ei::Vec2> points;
    points.reserve(numPoints);

    // Seed for random number generator
    std::srand(static_cast<unsigned>(std::time(0)));

    // Define the range within the window boundaries
    float minX = 100.0f;
    float minY = 100.0f;
    float maxX = WINDOW_DEFAULT_WIDTH - 100.0f;
    float maxY = WINDOW_DEFAULT_HEIGHT - 100.0f;

    // Randomly choose the line type: 0 = horizontal, 1 = vertical, 2 = diagonal
    int lineType = std::rand() % 3;

    float x0, y0, x1, y1;

    switch (lineType) {
    case 0: // Horizontal Line
        std::cout << "Generating horizontal line..." << std::endl;
        y0 = y1 = minY + static_cast<float>(std::rand()) / RAND_MAX * (maxY - minY);
        x0 = minX;
        x1 = maxX;
        break;
    case 1: // Vertical Line
        std::cout << "Generating vertical line..." << std::endl;
        x0 = x1 = minX + static_cast<float>(std::rand()) / RAND_MAX * (maxX - minX);
        y0 = minY;
        y1 = maxY;
        break;
    case 2: // Diagonal Line (random angle)
        std::cout << "Generating diagonal line (random angle)..." << std::endl;
        x0 = minX + static_cast<float>(std::rand()) / RAND_MAX * (maxX - minX);
        y0 = minY + static_cast<float>(std::rand()) / RAND_MAX * (maxY - minY);

        // Randomly pick an angle between -45 to 45 degrees (converted to radians)
        float angle = -0.785398f + static_cast<float>(std::rand()) / RAND_MAX * 1.5708f;

        // Calculate endpoint using the chosen angle and the line's length
        float length = std::min(maxX - x0, maxY - y0) * 0.8f; // Adjust length to fit within bounds
        x1 = x0 + length * std::cos(angle);
        y1 = y0 + length * std::sin(angle);
        break;
    }

    // Generate points along the line
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

    // Determine the size and position of the square
    float square_size = std::min(WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT) - 200.0f; // Leave margin
    float x0 = (WINDOW_DEFAULT_WIDTH - square_size) / 2.0f;
    float y0 = (WINDOW_DEFAULT_HEIGHT - square_size) / 2.0f;

    // Add the four corner points of the square
    points.emplace_back(x0, y0);                           // Top-left corner
    points.emplace_back(x0 + square_size, y0);              // Top-right corner
    points.emplace_back(x0, y0 + square_size);              // Bottom-left corner
    points.emplace_back(x0 + square_size, y0 + square_size);// Bottom-right corner

    // Adjust the number of random points to generate
    if (numPoints < 4) return points; // If fewer points are requested, only return the corners.
    size_t randomPointsToGenerate = numPoints - 4;

    // Set up random number generation within the square bounds
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distX(x0, x0 + square_size);
    std::uniform_real_distribution<float> distY(y0, y0 + square_size);

    // Generate random points inside the square
    for (size_t i = 0; i < randomPointsToGenerate; ++i) {
        float x = distX(gen);
        float y = distY(gen);
        points.emplace_back(x, y);
    }

    return points;
}
