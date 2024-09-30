#include "JarvisMarch.h"

short check_orientation(ei::Vec2 a, ei::Vec2 b, ei::Vec2 c)
{
    int orientation = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);

    if (orientation == 0)
        return ORIENTATION_COLLINEAR;
    return (orientation > 0) ? ORIENTATION_CLOCKWISE : ORIENTATION_COUNTERCLOCKWISE;
}

bool on_segment(ei::Vec2 a, ei::Vec2 b, ei::Vec2 c) 
{ 
    return (b.x <= std::max(a.x, c.x) && b.x >= std::min(a.x, c.x) && 
            b.y <= std::max(a.y, c.y) && b.y >= std::min(a.y, c.y));
       
} 

std::vector<ei::Vec2> jarvis_march_performance(INPUT_PARAMETER points)
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

AlgorithmGenerator jarvis_march_visualization(INPUT_PARAMETER points)
{
    unsigned int point_count = points.size();
    
    //if (point_count <= 3)
        //return points;

    //std::vector<ei::Vec2> hull;

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
        //hull.push_back(points[current]);
        ++hull_count;

        first_hull = point_count-hull_count+1;
        next = (current + 1) % first_hull;

        co_yield points[next];

        if (hull_count > 1) {
            help = points[first_hull];
            points[first_hull] = points[current];
            points[current] = help;
            current = first_hull;
        }

        for (unsigned int i = 0; i < first_hull; i++)
        {
            co_yield points[i];

            orientation = check_orientation(points[current], points[i], points[next]);
            if (orientation == ORIENTATION_CLOCKWISE) {
                next = i;
                co_yield points[next];
            }
            else if (orientation == ORIENTATION_COLLINEAR && on_segment(points[current], points[next], points[i]) && current != i) {
                next = i;
                co_yield points[next];
            }
        }

        current = next;

        co_yield points[current];

    } while (current != left);

    co_return;
}