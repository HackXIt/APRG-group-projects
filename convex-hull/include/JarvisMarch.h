//
// Header Supplied by RINI on 30/09/2024.
// Code by Marius
//

#ifndef JARVISMARCH_H
#define JARVISMARCH_H
#include "VisualAlgorithm.h"

const short ORIENTATION_COLLINEAR = 0;
const short ORIENTATION_CLOCKWISE = 1;
const short ORIENTATION_COUNTERCLOCKWISE = -1;

// Only added public functions in header
std::vector<ei::Vec2> jarvis_march_performance(INPUT_PARAMETER points);
AlgorithmGenerator jarvis_march_visualization(INPUT_PARAMETER points);

#endif //JARVISMARCH_H
