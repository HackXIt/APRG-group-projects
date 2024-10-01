//
// Created by RINI on 01/10/2024.
//

#include "DivideAndConquer.h"
#include <algorithm>
#include <stack>

// Helper function to compute the orientation of three points
short check_orientation(const ei::Vec2& a, const ei::Vec2& b, const ei::Vec2& c)
{
    float orientation = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);

    if (orientation == 0.0f)
        return ORIENTATION_COLLINEAR;
    return (orientation > 0.0f) ? ORIENTATION_CLOCKWISE : ORIENTATION_COUNTERCLOCKWISE;
}

// Compute the convex hull of three or fewer points
std::vector<ei::Vec2> computeHullOfThreePoints(const std::vector<ei::Vec2>& points)
{
    if (points.size() == 1)
        return points;

    if (points.size() == 2)
    {
        if (points[0] != points[1])
            return points;
        else
            return { points[0] };
    }

    // For three points
    const ei::Vec2& a = points[0];
    const ei::Vec2& b = points[1];
    const ei::Vec2& c = points[2];

    int orientation = check_orientation(a, b, c);

    if (orientation == ORIENTATION_COLLINEAR)
    {
        std::vector<ei::Vec2> temp = { a, b, c };
        // Sort points by x and y
        std::sort(temp.begin(), temp.end(), [](const ei::Vec2& p1, const ei::Vec2& p2){
            return p1.x < p2.x || (p1.x == p2.x && p1.y < p2.y);
        });
        // Return the two extreme points
        if (temp[0] != temp[2])
            return { temp[0], temp[2] };
        else
            return { temp[0] };
    }
    else
    {
        // Return points in counter-clockwise order
        if (orientation == ORIENTATION_CLOCKWISE)
            return { a, c, b };
        else
            return { a, b, c };
    }
}

// Merge two convex hulls into one
std::vector<ei::Vec2> mergeHulls(const std::vector<ei::Vec2>& leftHull, const std::vector<ei::Vec2>& rightHull)
{
    int n1 = leftHull.size();
    int n2 = rightHull.size();

    // Find the rightmost point of left hull
    int i = 0;
    for (int k = 1; k < n1; ++k)
    {
        if (leftHull[k].x > leftHull[i].x || (leftHull[k].x == leftHull[i].x && leftHull[k].y > leftHull[i].y))
        {
            i = k;
        }
    }

    // Find the leftmost point of right hull
    int j = 0;
    for (int k = 1; k < n2; ++k)
    {
        if (rightHull[k].x < rightHull[j].x || (rightHull[k].x == rightHull[j].x && rightHull[k].y < rightHull[j].y))
        {
            j = k;
        }
    }

    // Find upper tangent
    int i_upper = i;
    int j_upper = j;
    bool done = false;
    while (!done)
    {
        done = true;
        while (check_orientation(rightHull[j_upper], leftHull[i_upper], leftHull[(i_upper + 1) % n1]) == ORIENTATION_COUNTERCLOCKWISE)
        {
            i_upper = (i_upper + 1) % n1;
        }
        while (check_orientation(leftHull[i_upper], rightHull[j_upper], rightHull[(j_upper - 1 + n2) % n2]) == ORIENTATION_CLOCKWISE)
        {
            j_upper = (j_upper - 1 + n2) % n2;
            done = false;
        }
    }

    // Find lower tangent
    int i_lower = i;
    int j_lower = j;
    done = false;
    while (!done)
    {
        done = true;
        while (check_orientation(leftHull[i_lower], rightHull[j_lower], rightHull[(j_lower + 1) % n2]) == ORIENTATION_COUNTERCLOCKWISE)
        {
            j_lower = (j_lower + 1) % n2;
        }
        while (check_orientation(rightHull[j_lower], leftHull[i_lower], leftHull[(i_lower - 1 + n1) % n1]) == ORIENTATION_CLOCKWISE)
        {
            i_lower = (i_lower - 1 + n1) % n1;
            done = false;
        }
    }

    // Construct the merged hull
    std::vector<ei::Vec2> mergedHull;

    // Points from leftHull between i_upper and i_lower
    int idx = i_upper;
    mergedHull.push_back(leftHull[idx]);
    while (idx != i_lower)
    {
        idx = (idx + 1) % n1;
        mergedHull.push_back(leftHull[idx]);
    }

    // Points from rightHull between j_lower and j_upper
    idx = j_lower;
    mergedHull.push_back(rightHull[idx]);
    while (idx != j_upper)
    {
        idx = (idx + 1) % n2;
        mergedHull.push_back(rightHull[idx]);
    }

    return mergedHull;
}

// Recursive function to compute convex hull
std::vector<ei::Vec2> divideAndConquerHull(const std::vector<ei::Vec2>& points, int left, int right)
{
    // Base case
    if (left > right)
        return {};

    if (right - left + 1 <= 3)
    {
        std::vector<ei::Vec2> tempHull(points.begin() + left, points.begin() + right + 1);
        return computeHullOfThreePoints(tempHull);
    }

    // Recursive case
    int mid = (left + right) / 2;
    auto leftHull = divideAndConquerHull(points, left, mid);
    auto rightHull = divideAndConquerHull(points, mid + 1, right);

    // Merge leftHull and rightHull
    return mergeHulls(leftHull, rightHull);
}

std::vector<ei::Vec2> divide_and_conquer_performance(INPUT_PARAMETER& points)
{
    if (points.size() <= 1)
        return points;

    std::vector<ei::Vec2> sortedPoints = points;
    std::ranges::sort(sortedPoints, [](const ei::Vec2& a, const ei::Vec2& b){
        return a.x < b.x || (a.x == b.x && a.y < b.y);
    });

    return divideAndConquerHull(sortedPoints, 0, sortedPoints.size() - 1);
}

// Visualization code
// Visualization function
AlgorithmGenerator divide_and_conquer_visualization(INPUT_PARAMETER& points)
{
    auto visual = std::make_shared<Visual>();

    if (points.size() <= 1)
    {
        visual->setExplanation("Point cloud has less than 2 points. Convex hull is the point cloud itself.");
        for (const auto& point : points)
        {
            visual->addHullPoint(sf::Vector2f(point.x, point.y));
            visual->addHighlight(sf::Vector2f(point.x, point.y), "P", sf::Color::Blue);
            co_yield visual;
        }
        co_return;
    }

    // Sort points by x-coordinate
    visual->setExplanation("Sorting points by x-coordinate.");
    co_yield visual;

    std::vector<ei::Vec2> sortedPoints = points;
    std::sort(sortedPoints.begin(), sortedPoints.end(), [](const ei::Vec2& a, const ei::Vec2& b){
        return a.x < b.x || (a.x == b.x && a.y < b.y);
    });

    visual->setExplanation("Points sorted.");
    co_yield visual;

    // Implementing a non-recursive divide and conquer using a stack
    struct Frame {
        int left, right;
        bool merged;
        std::vector<ei::Vec2> hull;
    };

    std::stack<Frame> stack;
    stack.push({0, static_cast<int>(sortedPoints.size()) - 1, false, {}});

    std::map<std::pair<int, int>, std::vector<ei::Vec2>> hullMap;

    while (!stack.empty())
    {
        auto frame = stack.top();
        stack.pop();

        int left = frame.left;
        int right = frame.right;

        if (right - left + 1 <= 3)
        {
            // Base case: compute the convex hull directly
            std::vector<ei::Vec2> subset(sortedPoints.begin() + left, sortedPoints.begin() + right + 1);

            visual->setExplanation("Computing convex hull of small subset.");
            visual->clearHighlights();
            visual->clearHullPoints();
            for (const auto& p : subset)
            {
                visual->addHighlight(sf::Vector2f(p.x, p.y), "Point", sf::Color::Yellow);
            }
            co_yield visual;

            std::vector<ei::Vec2> hull;

            if (subset.size() == 1)
            {
                hull = subset;
            }
            else if (subset.size() == 2)
            {
                if (subset[0] != subset[1])
                    hull = subset;
                else
                    hull = { subset[0] };
            }
            else
            {
                const ei::Vec2& a = subset[0];
                const ei::Vec2& b = subset[1];
                const ei::Vec2& c = subset[2];

                int orientation = check_orientation(a, b, c);

                if (orientation == ORIENTATION_COLLINEAR)
                {
                    // Points are collinear
                    std::vector<ei::Vec2> temp = { a, b, c };
                    std::sort(temp.begin(), temp.end(), [](const ei::Vec2& p1, const ei::Vec2& p2){
                        return p1.x < p2.x || (p1.x == p2.x && p1.y < p2.y);
                    });

                    if (temp[0] != temp[2])
                        hull = { temp[0], temp[2] };
                    else
                        hull = { temp[0] };
                }
                else
                {
                    // Points form a triangle
                    if (orientation == ORIENTATION_CLOCKWISE)
                        hull = { a, c, b };
                    else
                        hull = { a, b, c };
                }
            }

            // Visualize the computed hull
            visual->setExplanation("Computed hull of subset.");
            visual->clearHighlights();
            visual->clearHullPoints();
            for (const auto& p : hull)
            {
                visual->addHullPoint(sf::Vector2f(p.x, p.y));
            }
            co_yield visual;

            // Store the hull
            hullMap[{left, right}] = hull;
        }
        else
        {
            if (!frame.merged)
            {
                // Divide the set
                int mid = (left + right) / 2;

                // Visualize division
                visual->setExplanation("Dividing the set into two halves.");
                visual->addIndicatorLine(
                    sf::Vector2f(sortedPoints[mid].x, -10000), sf::Vector2f(sortedPoints[mid].x, 10000), sf::Color::Green);
                co_yield visual;
                visual->clearIndicatorLines();

                // Push current frame back to stack to process after children
                frame.merged = true;
                stack.push(frame);

                // Push right and left halves
                stack.push({mid + 1, right, false, {}});
                stack.push({left, mid, false, {}});
            }
            else
            {
                // Merge the two halves
                int mid = (left + right) / 2;
                auto leftHull = hullMap[{left, mid}];
                auto rightHull = hullMap[{mid + 1, right}];

                // Visualize the two hulls
                visual->setExplanation("Merging left and right hulls.");
                visual->clearHighlights();
                visual->clearHullPoints();
                for (const auto& p : leftHull)
                {
                    visual->addHullPoint(sf::Vector2f(p.x, p.y), sf::Color::Red);
                }
                for (const auto& p : rightHull)
                {
                    visual->addHullPoint(sf::Vector2f(p.x, p.y), sf::Color::Blue);
                }
                co_yield visual;

                // Merging process
                int n1 = leftHull.size();
                int n2 = rightHull.size();

                // Find rightmost point of left hull
                int i = 0;
                for (int k = 1; k < n1; ++k)
                {
                    if (leftHull[k].x > leftHull[i].x)
                        i = k;
                }

                // Find leftmost point of right hull
                int j = 0;
                for (int k = 1; k < n2; ++k)
                {
                    if (rightHull[k].x < rightHull[j].x)
                        j = k;
                }

                // Find upper tangent
                int i_upper = i;
                int j_upper = j;
                bool done = false;
                while (!done)
                {
                    done = true;
                    while (check_orientation(rightHull[j_upper], leftHull[i_upper], leftHull[(i_upper + 1) % n1]) == ORIENTATION_COUNTERCLOCKWISE)
                    {
                        i_upper = (i_upper + 1) % n1;

                        // Visualize adjusting upper tangent
                        visual->setExplanation("Adjusting upper tangent (left hull).");
                        visual->addHighlight(sf::Vector2f(leftHull[i_upper].x, leftHull[i_upper].y), "Upper Tangent", sf::Color::Yellow);
                        co_yield visual;
                        visual->removeHighlight(sf::Vector2f(leftHull[i_upper].x, leftHull[i_upper].y));
                    }
                    while (check_orientation(leftHull[i_upper], rightHull[j_upper], rightHull[(j_upper - 1 + n2) % n2]) == ORIENTATION_CLOCKWISE)
                    {
                        j_upper = (j_upper - 1 + n2) % n2;
                        done = false;

                        // Visualize adjusting upper tangent
                        visual->setExplanation("Adjusting upper tangent (right hull).");
                        visual->addHighlight(sf::Vector2f(rightHull[j_upper].x, rightHull[j_upper].y), "Upper Tangent", sf::Color::Cyan);
                        co_yield visual;
                        visual->removeHighlight(sf::Vector2f(rightHull[j_upper].x, rightHull[j_upper].y));
                    }
                }

                // Find lower tangent
                int i_lower = i;
                int j_lower = j;
                done = false;
                while (!done)
                {
                    done = true;
                    while (check_orientation(leftHull[i_lower], rightHull[j_lower], rightHull[(j_lower + 1) % n2]) == ORIENTATION_COUNTERCLOCKWISE)
                    {
                        j_lower = (j_lower + 1) % n2;

                        // Visualize adjusting lower tangent
                        visual->setExplanation("Adjusting lower tangent (right hull).");
                        visual->addHighlight(sf::Vector2f(rightHull[j_lower].x, rightHull[j_lower].y), "Lower Tangent", sf::Color::Cyan);
                        co_yield visual;
                        visual->removeHighlight(sf::Vector2f(rightHull[j_lower].x, rightHull[j_lower].y));
                    }
                    while (check_orientation(rightHull[j_lower], leftHull[i_lower], leftHull[(i_lower - 1 + n1) % n1]) == ORIENTATION_CLOCKWISE)
                    {
                        i_lower = (i_lower - 1 + n1) % n1;
                        done = false;

                        // Visualize adjusting lower tangent
                        visual->setExplanation("Adjusting lower tangent (left hull).");
                        visual->addHighlight(sf::Vector2f(leftHull[i_lower].x, leftHull[i_lower].y), "Lower Tangent", sf::Color::Yellow);
                        co_yield visual;
                        visual->removeHighlight(sf::Vector2f(leftHull[i_lower].x, leftHull[i_lower].y));
                    }
                }

                // Merge the hulls
                std::vector<ei::Vec2> mergedHull;

                // From leftHull[i_upper] to leftHull[i_lower]
                int idx = i_upper;
                mergedHull.push_back(leftHull[idx]);
                while (idx != i_lower)
                {
                    idx = (idx + 1) % n1;
                    mergedHull.push_back(leftHull[idx]);
                }

                // From rightHull[j_lower] to rightHull[j_upper]
                idx = j_lower;
                mergedHull.push_back(rightHull[idx]);
                while (idx != j_upper)
                {
                    idx = (idx + 1) % n2;
                    mergedHull.push_back(rightHull[idx]);
                }

                // Visualize the merged hull
                visual->setExplanation("Merged hull after combining.");
                visual->clearHighlights();
                visual->clearHullPoints();
                for (const auto& p : mergedHull)
                {
                    visual->addHullPoint(sf::Vector2f(p.x, p.y), sf::Color::Green);
                }
                co_yield visual;

                // Store the merged hull
                hullMap[{left, right}] = mergedHull;

                // If this is the final merge
                if (left == 0 && right == static_cast<int>(sortedPoints.size()) - 1)
                {
                    visual->setExplanation("Final convex hull constructed.");
                    visual->finished = true;
                    co_yield visual;
                    co_return;
                }
            }
        }
    }
}