//
// Created by RINI on 22/09/2024.
//

#include "IntersectionTestsFromBook.h"

#include <cmath>

// AFTER finding the epsilon-intersection library, I gave up on further writing out functions from the book

// NOTE All functions have commented out calculations for Z-axis, since our showcase is 2D

namespace collision_detection {

    //calculates dot product of 2 vectors
    float dot( const sf::Vector2f& lhs, const sf::Vector2f& rhs ) noexcept
    {
        return lhs.x * rhs.x + lhs.y + rhs.y;
    }

    //Returns length of a given vector
    float length(const sf::Vector2f& source)
    {
        return sqrtf(dot(source, source));
    }

    void MostSeparatedPointsOnAABB(int &min, int &max, sf::Vector2f pt[], int numPts)
    {
        // First find most extreme points along principal axes
        int minx = 0, maxx = 0, miny = 0, maxy = 0, minz = 0, maxz = 0;
        for (int i = 1; i < numPts; i++) {
            if (pt[i].x < pt[minx].x) minx = i;
            if (pt[i].x > pt[maxx].x) maxx = i;
            if (pt[i].y < pt[miny].y) miny = i;
            if (pt[i].y > pt[maxy].y) maxy = i;
            // if (pt[i].z < pt[minz].z) minz = i;
            // if (pt[i].z > pt[maxz].z) maxz = i;
        }
        // Compute the squared distances for the three pairs of points
        float dist2x = dot(pt[maxx] - pt[minx], pt[maxx] - pt[minx]);
        float dist2y = dot(pt[maxy] - pt[miny], pt[maxy] - pt[miny]);
        // float dist2z = Dot(pt[maxz] - pt[minz], pt[maxz] - pt[minz]);
        // Pick the pair (min,max) of points most distant
        min = minx;
        max = maxx;
        // if (dist2y > dist2x && dist2y > dist2z) {
        //     max = maxy;
        //     min = miny;
        // }
        // if (dist2z > dist2x && dist2z > dist2y) {
        //     max = maxz;
        //     min = minz;
        // }
    }
    void SphereFromDistantPoints(SphereBV &s, sf::Vector2f pt[], int numPts)
    {
        // Find the most separated point pair defining the encompassing AABB
        int min, max;
        MostSeparatedPointsOnAABB(min, max, pt, numPts);
        // Set up sphere to just encompass these two points
        s.c = (pt[min] + pt[max]) * 0.5f;
        s.r = dot(pt[max] - s.c, pt[max] - s.c);
        s.r = sqrtf(s.r);
    }

    // Given Sphere s and Point p, update s (if needed) to just encompass p
    void SphereOfSphereAndPt(SphereBV &s, sf::Vector2f &p)
    {
        // Compute squared distance between point and sphere center
        sf::Vector2f d = p - s.c;
        float dist2 = dot(d, d);
        // Only update s if point p is outside it
        if (dist2 > s.r * s.r) {
            float dist = sqrtf(dist2);
            float newRadius = (s.r + dist) * 0.5f;
            float k = (newRadius - s.r) / dist;
            s.r = newRadius;
            s.c += d * k;
        }
    }

    void RitterSphere(SphereBV &s, sf::Vector2f pt[], int numPts)
    {
        // Get sphere encompassing two approximately most distant points
        SphereFromDistantPoints(s, pt, numPts);
        // Grow sphere to include all points
        for (int i = 0; i < numPts; i++)
            SphereOfSphereAndPt(s, pt[i]);
    }

    // Compute variance of a set of 1D values
    float Variance(float x[], int n)
    {
        float u = 0.0f;
        for (int i = 0; i < n; i++)
            u += x[i];
        u /= n;
        float s2 = 0.0f;
        for (int i = 0; i < n; i++)
            s2 += (x[i] - u) * (x[i] - u);
        return s2 / n;
    }


    int TestAABBAABB(AABB a, AABB b)
    {
        // Exit with no intersection if separated along an axis
        if (a.max.position.x < b.min.position.x || a.min.position.x > b.max.position.x) return 0;
        if (a.max.position.y < b.min.position.y || a.min.position.y > b.max.position.y) return 0;
        // No Z-axis in our 2D showcase
        //if (a.max[2] < b.min[2] || a.min[2] > b.max[2]) return 0;
        // Overlapping on all axes means AABBs are intersecting
        return 1;
    }

    int TestSphereSphere(SphereBV a, SphereBV b)
    {
        sf::Vector2f d = a.c - b.c;
        float dist2 = dot(d, d);
        float radiusSum = a.r + b.r;
        return dist2 <= radiusSum * radiusSum;
    }

    /*
    int TestMovingSphereSphere(SphereBV s0, sf::Vector2f d, float t0, float t1, SphereBV s1, float &t)
    {
        // Compute sphere bounding motion of s0 during time interval from t0 to t1
        SphereBV b;
        float mid = (t0 + t1) * 0.5f;
        b.c = s0.c + d * mid;
        b.r = (mid – t0) * length(d) + s0.r;
        // If bounding sphere not overlapping s1, then no collision in this interval
        if (!TestSphereSphere(b, s1)) return 0;
        // Cannot rule collision out: recurse for more accurate testing. To terminate the
        // recursion, collision is assumed when time interval becomes sufficiently small
        if (t1 - t0 < INTERVAL_EPSILON) {
            t = t0;
            return 1;
        }
        // Recursively test first half of interval; return collision if detected
        if (TestMovingSphereSphere(s0, d, t0, mid, s1, t)) return 1;
        // Recursively test second half of interval
        return TestMovingSphereSphere(s0, d, mid, t1, s1, t);
    }
    */

} // collision_detection