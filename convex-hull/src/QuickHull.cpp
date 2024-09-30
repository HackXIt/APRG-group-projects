#include "QuickHull.h"

inline int pointLocation(const ei::Vec2& A, const ei::Vec2& B, const ei::Vec2& P) {
    float cp1 = (B.x - A.x)*(P.y - A.y) - (B.y - A.y)*(P.x - A.x);
    if (cp1 > 0.0f)
        return 1;
    else if (cp1 == 0.0f)
        return 0;
    else
        return -1;
}

inline float distanceFromLine(const ei::Vec2& A, const ei::Vec2& B, const ei::Vec2& P) {
    float num = std::abs((B.y - A.y)*P.x - (B.x - A.x)*P.y + B.x*A.y - B.y*A.x);
    float den = std::sqrt((B.y - A.y)*(B.y - A.y) + (B.x - A.x)*(B.x - A.x)); //sqrt can be optimised
    return num / den;
}

#include <stack>


//P1 (Point1 left), P2 (Point2 right), C (New Point center)
AlgorithmGenerator quickHullAlgorithm(const INPUT_PARAMETER& points)
{
    if (points.size() <3) {
        for (const auto& point : points) {
            co_yield point;
        }
        co_return;
    }

    auto minmaxX = std::minmax_element(points.begin(), points.end(),
        [](const ei::Vec2& a, const ei::Vec2& b){return a.x < b.x; });

    ei::Vec2 A = *minmaxX.first;
    ei::Vec2 B = *minmaxX.second;

    co_yield A;
    co_yield B;

    std::vector<ei::Vec2> leftSet;
    std::vector<ei::Vec2> rightSet;

    for (const auto& point : points) {
        if (point == A || point == B) continue;
        if (pointLocation(A, B, point) == 1)
            leftSet.push_back(point);
        else if (pointLocation(A, B, point) == -1)
            rightSet.push_back(point);
    }

    //wegen yield wird rec nur simuliert.
    std::stack<std::tuple<ei::Vec2, ei::Vec2, std::vector<ei::Vec2>>> stack;

    std::vector<ei::Vec2> convexHull = {A, B};

    stack.push(std::make_tuple(A, B, leftSet));
    stack.push(std::make_tuple(B, A, rightSet));

    while (!stack.empty()) {
        auto [P1, P2, set] = stack.top();
        stack.pop();

        if (set.empty())
            continue;

        float maxDistance = -1.0f;
        ei::Vec2 C;

        for (const auto& point : set) {
            float distance = distanceFromLine(P1, P2, point);
            if (distance > maxDistance) {
                maxDistance = distance;
                C = point;
            }
        }

        co_yield C;

        auto it = std::find(convexHull.begin(), convexHull.end(), P2);
        convexHull.insert(it, C);


        std::vector<ei::Vec2> leftSet1;
        std::vector<ei::Vec2> leftSet2;

        for (const auto& point : set) {
            if (point == C)
                continue;
            if (pointLocation(P1, C, point) == 1)
                leftSet1.push_back(point);
            else if (pointLocation(C, P2, point) == 1)
                leftSet2.push_back(point);
        }

        stack.push(std::make_tuple(C, P2, leftSet2));
        stack.push(std::make_tuple(P1, C, leftSet1));
    }

    for (const auto& point : convexHull) {
        co_yield point;
    }

    co_return;
}