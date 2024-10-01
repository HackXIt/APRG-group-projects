#include "JarvisMarch.h"

const float eps = 0.000001;

short check_orientation(ei::Vec2 a, ei::Vec2 b, ei::Vec2 c)
{
    float orientation = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);

    if (orientation <= 0.0f + eps && orientation >= 0.0f - eps)
        return ORIENTATION_COLLINEAR;
    return (orientation > 0.0f) ? ORIENTATION_CLOCKWISE : ORIENTATION_COUNTERCLOCKWISE;
}

bool on_segment(ei::Vec2 a, ei::Vec2 b, ei::Vec2 c) 
{ 
    return (b.x <= std::max(a.x, c.x) && b.x >= std::min(a.x, c.x) && 
            b.y <= std::max(a.y, c.y) && b.y >= std::min(a.y, c.y));
       
} 

std::vector<ei::Vec2> jarvis_march_performance(INPUT_PARAMETER& points)
{
    unsigned int point_count = points.size();
    
    if (point_count <= 3)
        return points;

    std::vector<ei::Vec2> hull;

    unsigned int left = 0;
    for (unsigned int i = 1; i < point_count; i++)
        if (points[i].x < points[left].x)
            left = i;

    unsigned int current = left;
    unsigned int next;

    unsigned int hull_count = 0;
    unsigned int first_hull = 0;
    ei::Vec2 help;

    short orientation;

    do {
        hull.push_back(points[current]);
        ++hull_count;

        first_hull = point_count-hull_count+1;
        next = (current + 1) % (first_hull);

        if (hull_count > 1) {
            help = points[first_hull];
            points[first_hull] = points[current];
            points[current] = help;
            if (first_hull == left)
                left = current;
            current = first_hull;
        }

        for (unsigned int i = 0; i < first_hull; i++)
        {
            orientation = check_orientation(points[current], points[i], points[next]);
            if (orientation == ORIENTATION_CLOCKWISE)
                next = i;
            else if (orientation == ORIENTATION_COLLINEAR && on_segment(points[current], points[next], points[i]) && current != i)
                next = i;
        }

        current = next;

    } while (current != left);

    return hull;
}

AlgorithmGenerator jarvis_march_visualization(INPUT_PARAMETER& points)
{
    auto visual = std::make_shared<Visual>();
    unsigned int point_count = points.size();

    if (point_count < 3)
    {
        visual->setExplanation("Point cloud has less than 3 points. Convex hull is the point cloud itself.");
        for (const auto& point : points)
        {
            visual->current_hull.append(sf::Vertex(sf::Vector2f(point.x, point.y), sf::Color::Green));
            visual->addHighlight(sf::Vector2f(point.x, point.y), "P", sf::Color::Blue);
            co_yield visual;
        }
        co_return;
    }

    // Find the leftmost point
    visual->setExplanation("Finding the leftmost point.");
    unsigned int leftmost = 0;
    visual->addHighlight(sf::Vector2f(points[leftmost].x, points[leftmost].y), "Initial Point", sf::Color::Red);
    co_yield visual;

    for (unsigned int i = 1; i < point_count; i++)
    {
        if (points[i].x < points[leftmost].x)
        {
            // Remove previous highlight
            visual->removeHighlight(sf::Vector2f(points[leftmost].x, points[leftmost].y));

            leftmost = i;
            visual->addHighlight(sf::Vector2f(points[leftmost].x, points[leftmost].y), "New Leftmost Point", sf::Color::Red);
            co_yield visual;
        }
    }
    visual->clearHighlights();
    visual->setExplanation("Found leftmost as starting point for algorithm.");
    visual->addHighlight(sf::Vector2f(points[leftmost].x, points[leftmost].y), "The Leftmost Point", sf::Color::Magenta);
    co_yield visual;
    visual->clearHighlights();

    std::vector<ei::Vec2> convexHull;
    unsigned int p = leftmost, q;

    do
    {
        // Highlight current point p
        visual->addHighlight(sf::Vector2f(points[p].x, points[p].y), "Hull Point P", sf::Color::Red);
        visual->setExplanation("Hull point P selected.");
        convexHull.push_back(points[p]);
        // Visualize current convex hull
        visual->current_hull.clear();
        for (const auto& point : convexHull)
        {
            visual->current_hull.append(sf::Vertex(sf::Vector2f(point.x, point.y), sf::Color::Red));
        }
        co_yield visual;

        q = (p + 1) % point_count;

        // Highlight candidate point q
        visual->addHighlight(sf::Vector2f(points[q].x, points[q].y), "Candidate Q", sf::Color::Yellow);
        visual->setExplanation("Selected candidate point Q.");
        co_yield visual;

        for (unsigned int i = 0; i < point_count; i++)
        {
            if (i == p || i == q)
                continue;

            // Clear indicator lines
            visual->indicator_lines.clear();

            // Draw indicator lines between p-q and p-i
            visual->indicator_lines.emplace_back(sf::Vector2f(points[p].x, points[p].y), sf::Vector2f(points[q].x, points[q].y), sf::Color::Blue);
            visual->indicator_lines.emplace_back(sf::Vector2f(points[p].x, points[p].y), sf::Vector2f(points[i].x, points[i].y), sf::Color::Green);

            // Highlight point i being considered
            visual->addHighlight(sf::Vector2f(points[i].x, points[i].y), "Probe I", sf::Color::Cyan);
            visual->setExplanation("Checking orientation between P, I, and Q.");
            co_yield visual;
            visual->removeHighlight(sf::Vector2f(points[i].x, points[i].y));

            int orientation = check_orientation(points[p], points[i], points[q]);

            if (orientation == ORIENTATION_COUNTERCLOCKWISE)
            {
                // Remove previous highlight for q
                visual->removeHighlight(sf::Vector2f(points[q].x, points[q].y));

                // Update q
                q = i;

                // Highlight new candidate q
                visual->addHighlight(sf::Vector2f(points[q].x, points[q].y), "New Candidate Q", sf::Color::Yellow);
                visual->setExplanation("Found more counterclockwise point. Updating Q.");
                co_yield visual;
            }
        }

        // Clear after comparisons
        visual->indicator_lines.clear();
        visual->clearHighlights();

        // Move to next point
        p = q;
        visual->addHighlight(sf::Vector2f(points[p].x, points[p].y), "Next Point P", sf::Color::Magenta);
        // Optional: Yield to show movement to next point
        visual->setExplanation("P is on Hull. Selected as next point P.");
        co_yield visual;
        visual->removeHighlight(sf::Vector2f(points[p].x, points[p].y));
    } while (p != leftmost); // While we don't come back to the first point

    // Final convex hull visualization
    visual->clearHighlights();
    visual->indicator_lines.clear();
    visual->current_hull.clear();
    for (const auto& point : convexHull)
    {
        visual->current_hull.append(sf::Vertex(sf::Vector2f(point.x, point.y), sf::Color::Red));
    }
    // Close the loop by adding the first point at the end
    visual->current_hull.append(sf::Vertex(sf::Vector2f(convexHull.front().x, convexHull.front().y), sf::Color::Red));
    visual->setExplanation("Convex hull construction complete.");
    visual->finished = true;
    co_yield visual;

    co_return;
}
