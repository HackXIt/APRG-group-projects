#ifndef QUICKHULL_H
#define QUICKHULL_H
#include "VisualAlgorithm.h"

//std::vector<ei::Vec2> computeHull(const std::vector<ei::Vec2>& points);
std::vector<ei::Vec2> quick_hull_performance(const INPUT_PARAMETER& points);
AlgorithmGenerator quick_hull_visualization(const INPUT_PARAMETER& points);


void findHull(std::vector<ei::Vec2>& hull, const std::vector<ei::Vec2>& set,
              const ei::Vec2& P, const ei::Vec2& Q);

int pointLocation(const ei::Vec2& A, const ei::Vec2& B, const ei::Vec2& P);
float distanceFromLine(const ei::Vec2& A, const ei::Vec2& B, const ei::Vec2& P);
float distanceSquaredFromLine(const ei::Vec2& A, const ei::Vec2& B, const ei::Vec2& P);

#endif //QUICKHULL_H
