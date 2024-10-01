#include <memory>

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

std::vector<ei::Vec2> quick_hull_performance(const INPUT_PARAMETER& points)
{
    if (points.size() < 3)
    {
        return points;
    }

    auto minmaxX = std::minmax_element(points.begin(), points.end(),
        [](const ei::Vec2& a, const ei::Vec2& b){return a.x < b.x; });

    ei::Vec2 A = *minmaxX.first;
    ei::Vec2 B = *minmaxX.second;

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

    std::vector convexHull = {A, B};

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

    return convexHull;
}

AlgorithmGenerator quick_hull_visualization(const INPUT_PARAMETER& points)
{
    auto visual = std::make_shared<Visual>();

    if (points.size() < 3)
    {
        visual->setExplanation("Point cloud has less than 3 points. Convex hull is the point cloud itself.");
        for (const auto& point : points)
        {
            visual->addHullPoint(sf::Vector2f(point.x, point.y));
            visual->addHighlight(sf::Vector2f(point.x, point.y), "P", sf::Color::Blue);
            co_yield visual;
        }
        co_return;
    }

    auto minmaxX = std::minmax_element(points.begin(), points.end(),
        [](const ei::Vec2& a, const ei::Vec2& b) { return a.x < b.x; });

    ei::Vec2 A = *minmaxX.first;
    ei::Vec2 B = *minmaxX.second;

    visual->setExplanation("Finding minimum x-coordinate in point cloud.");
    visual->addHighlight(sf::Vector2f(A.x, A.y), "A (min point)", sf::Color::Red);
    co_yield visual;

    visual->setExplanation("Finding maximum x-coordinate in point cloud.");
    visual->addHighlight(sf::Vector2f(B.x, B.y), "B (max point)", sf::Color::Red);
    co_yield visual;

    visual->addIndicatorLine(sf::Vector2f(A.x, A.y), sf::Vector2f(B.x, B.y), sf::Color::Red);
    visual->setExplanation("Line AB is formed between the minimum and maximum x-coordinates.");
    co_yield visual;

    visual->setExplanation("Dividing the point cloud into two sets: left and right of line AB.");
    std::vector<ei::Vec2> leftSet;
    std::vector<ei::Vec2> rightSet;

    for (const auto& point : points)
    {
        if (point == A || point == B)
            continue;

        int location = pointLocation(A, B, point);
        if (location == 1)
        {
            leftSet.push_back(point);
            // Visualize the point being added to the left set
            visual->addHighlight(sf::Vector2f(point.x, point.y), "P in Left Set", sf::Color::Green);
            visual->setExplanation("Point is on the left side of line A-B.");
            co_yield visual;
        }
        else if (location == -1)
        {
            rightSet.push_back(point);
            // Visualize the point being added to the right set
            visual->addHighlight(sf::Vector2f(point.x, point.y), "P in Right Set", sf::Color::Blue);
            visual->setExplanation("Point is on the right side of line A-B.");
            co_yield visual;
        }
        // No need to visualize points on the line (location == 0) in this step
    }
    visual->clearHighlights();

    // Initialize convex hull with points A and B
    std::vector<ei::Vec2> convexHull = { A, B };
    visual->clearHullPoints();
    visual->addHullPoint(sf::Vector2f(A.x, A.y));
    visual->addHullPoint(sf::Vector2f(B.x, B.y));
    visual->setExplanation("Initializing convex hull with points A and B.");
    co_yield visual;

    // Initialize the stack for non-recursive processing
    std::stack<std::tuple<ei::Vec2, ei::Vec2, std::vector<ei::Vec2>>> stack;
    stack.push(std::make_tuple(A, B, leftSet));
    stack.push(std::make_tuple(B, A, rightSet));

    // Variables to keep track of previously highlighted points
    ei::Vec2 prevP1{}, prevP2{}, prevC{};

    // Begin the non-recursive process using a stack
    while (!stack.empty())
    {
        // Remove highlights from previous iteration
        if (prevP1 != ei::Vec2{})
            visual->removeHighlight(sf::Vector2f(prevP1.x, prevP1.y));
        if (prevP2 != ei::Vec2{})
            visual->removeHighlight(sf::Vector2f(prevP2.x, prevP2.y));
        if (prevC != ei::Vec2{})
            visual->removeHighlight(sf::Vector2f(prevC.x, prevC.y));

        visual->clearIndicatorLines(); // Clear previous indicator lines

        auto [P1, P2, set] = stack.top();
        stack.pop();

        // Visualize the current edge being processed
        visual->setExplanation("Processing edge between P1 and P2.");
        visual->addHighlight(sf::Vector2f(P1.x, P1.y), "P1", sf::Color::Magenta);
        visual->addHighlight(sf::Vector2f(P2.x, P2.y), "P2", sf::Color::Magenta);
        visual->addIndicatorLine(sf::Vector2f(P1.x, P1.y), sf::Vector2f(P2.x, P2.y), sf::Color::Magenta);
        co_yield visual;

        // Update previous P1 and P2
        prevP1 = P1;
        prevP2 = P2;

        if (set.empty())
        {
            visual->setExplanation("No points left on this side of the edge.");
            co_yield visual;
            continue;
        }

        // Find the point C with the maximum distance from the line P1P2
        float maxDistance = -1.0f;
        ei::Vec2 C;
        for (const auto& point : set)
        {
            float distance = distanceFromLine(P1, P2, point);
            if (distance > maxDistance)
            {
                maxDistance = distance;
                C = point;
            }
        }

        // Visualize point C
        visual->addHighlight(sf::Vector2f(C.x, C.y), "C (Furthest from edge)", sf::Color::Yellow);
        visual->setExplanation("Found point C with maximum distance from edge P1-P2.");
        co_yield visual;
        // Remove highlights for P1 and P2
        visual->removeHighlight(sf::Vector2f(P1.x, P1.y));
        visual->removeHighlight(sf::Vector2f(P2.x, P2.y));

        // Update previous C
        prevC = C;

        // Insert C into convex hull after P1
        auto it = std::find(convexHull.begin(), convexHull.end(), P1);
        if (it != convexHull.end()) {
            ++it; // Move iterator to position after P1
            convexHull.insert(it, C);
        } else {
            // This should not happen, but handle it just in case
            convexHull.push_back(C);
        }

        // Visualize updated convex hull
        visual->clearHullPoints();
        for (const auto& point : convexHull)
        {
            visual->addHullPoint(sf::Vector2f(point.x, point.y));
        }
        visual->setExplanation("Inserted point C into convex hull.");
        co_yield visual;
        // Remove highlight for C after updating the convex hull
        visual->removeHighlight(sf::Vector2f(C.x, C.y));

        // Determine points to the left of lines P1C and CP2
        std::vector<ei::Vec2> leftSet1;
        std::vector<ei::Vec2> leftSet2;

        for (const auto& point : set)
        {
            if (point == C)
                continue;

            int location1 = pointLocation(P1, C, point);
            if (location1 == 1)
            {
                leftSet1.push_back(point);
                // Visualize point added to leftSet1
                visual->addHighlight(sf::Vector2f(point.x, point.y), "P in Left Set 1", sf::Color::Green);
                visual->setExplanation("Point is on the left side of line P1-C.");
                co_yield visual;
                // visual->removeHighlight(sf::Vector2f(point.x, point.y)); // Remove highlight
            }
            else
            {
                int location2 = pointLocation(C, P2, point);
                if (location2 == 1)
                {
                    leftSet2.push_back(point);
                    // Visualize point added to leftSet2
                    visual->addHighlight(sf::Vector2f(point.x, point.y), "P in Left Set 2", sf::Color::Blue);
                    visual->setExplanation("Point is on the left side of line C-P2.");
                    co_yield visual;
                    // visual->removeHighlight(sf::Vector2f(point.x, point.y)); // Remove highlight
                }
            }
        }
        visual->clearHighlights();

        // Add new edges to the stack for further processing
        stack.push(std::make_tuple(P1, C, leftSet1));
        stack.push(std::make_tuple(C, P2, leftSet2));
        visual->setExplanation("Added new edges to stack for processing.");
        co_yield visual;
    }

    // Final convex hull visualization
    visual->clearHighlights();
    visual->clearHighlights();
    visual->clearHullPoints();
    for (const auto& point : convexHull)
    {
        visual->addHullPoint(sf::Vector2f(point.x, point.y));
    }
    // Close the loop by adding the first point at the end
    visual->addHullPoint(sf::Vector2f(convexHull.front().x, convexHull.front().y));
    visual->setExplanation("Convex hull construction complete.");
    visual->finished = true;
    co_yield visual;

    co_return;
}

