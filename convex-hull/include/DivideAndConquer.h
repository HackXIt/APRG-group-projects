//
// Created by RINI on 01/10/2024.
//

#ifndef DIVIDE_AND_CONQUER_H
#define DIVIDE_AND_CONQUER_H

#include <vector>
#include "VisualAlgorithm.h"
#include "ei/2dtypes.hpp"
#include "JarvisMarch.h"

#define INPUT_PARAMETER std::vector<ei::Vec2>

std::vector<ei::Vec2> divide_and_conquer_performance(INPUT_PARAMETER& points);

AlgorithmGenerator divide_and_conquer_visualization(INPUT_PARAMETER& points);

#endif // DIVIDE_AND_CONQUER_H