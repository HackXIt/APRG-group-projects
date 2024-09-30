#include "VisualAlgorithm.h"

const short ORIENTATION_COLLINEAR = 0;
const short ORIENTATION_CLOCKWISE = 1;
const short ORIENTATION_COUNTERCLOCKWISE = -1;

short check_orientation(ei::Vec2 a, ei::Vec2 b, ei::Vec2 c)
{
    int orientation = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);

    if (orientation == 0)
        return ORIENTATION_COLLINEAR;
    return (orientation > 0) ? ORIENTATION_CLOCKWISE : ORIENTATION_COUNTERCLOCKWISE;
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
    do
    {
        hull.push_back(points[current]);

        next = (current + 1) % point_count;
        for (unsigned int i = 0; i < point_count; i++)
        {
            if (check_orientation(points[current], points[i], points[next]) == ORIENTATION_CLOCKWISE)
                next = i;
        }

        current = next;

    }
    while (current != left);

    return hull;
}