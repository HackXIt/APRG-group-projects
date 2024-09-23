//
// Created by RINI on 22/09/2024.
//

#ifndef INTERSECTIONTESTS_H
#define INTERSECTIONTESTS_H
#include "Plane.h"
#include "Sphere.h"

// NOTE Sources taken from: Ericson, C. (2004). Real-time collision detection. Taylor & Francis Group.
// Adjusted for types available in SFML

namespace collision_detection_book {

#pragma region UtilityFunctions

    // Dot-product of 2 SFML vectors (since Dot(..) is used a lot in the book)
    float dot( const sf::Vector2f& lhs, const sf::Vector2f& rhs ) noexcept;

    // Compute indices to the two most separated points of the (up to) six points
    // defining the AABB encompassing the point set. Return these as min and max.
    void MostSeparatedPointsOnAABB(int &min, int &max, sf::Vector2f pt[], int numPts);
    void SphereFromDistantPoints(collision_detection::Sphere &s, sf::Vector2f pt[], int numPts);

#pragma endregion

#pragma region BoundingVolumeAABB
    // Multiple Axis-Aligned-Bounding-Box representations
    // Adjusted for SFML
    struct AABB {
        sf::Vertex min;
        sf::Vertex max;
    };
    /* Min-Max representation
    // region R = { (x, y, z) | min.x<=x<=max.x, min.y<=y<=max.y, min.z<=z<=max.z }
    struct AABB {
        Point min;
        Point max;
    };
    */
    /* min-width representation
    // Minimum corner + width/diameter extents representation
    // region R = { (x, y, z) | min.x<=x<=min.x+dx, min.y<=y<=min.y+dy, min.z<=z<=min.z+dz }
    struct AABB {
        Point min;
        float d[3]; // diameter or width extents (dx, dy, dz)
    };
    */
    /* center-radius representation
    // Center + half-size extents representation
    // region R = { (x, y, z) | |c.x-x|<=rx, |c.y-y|<=ry, |c.z-z|<=rz }
    struct AABB {
        Point c; // center point of AABB
        float r[3]; // radius or halfwidth extents (rx, ry, rz)
    };
    */

    int TestAABBAABB(AABB a, AABB b);
    /* min-max implementation from the book
    {
        // Exit with no intersection if separated along an axis
        if (a.max[0] < b.min[0] || a.min[0] > b.max[0]) return 0;
        if (a.max[1] < b.min[1] || a.min[1] > b.max[1]) return 0;
        if (a.max[2] < b.min[2] || a.min[2] > b.max[2]) return 0;
        // Overlapping on all axes means AABBs are intersecting
        return 1;
    }
    */
    /* min-width implementation from the book
    {
        float t;
        if ((t = a.min[0] - b.min[0]) > b.d[0] || -t > a.d[0]) return 0;
        if ((t = a.min[1] - b.min[1]) > b.d[1] || -t > a.d[1]) return 0;
        if ((t = a.min[2] - b.min[2]) > b.d[2] || -t > a.d[2]) return 0;
        return 1;
    }
    */
    /* center-radius implementation from the book
    {
        if (Abs(a.c[0] - b.c[0]) > a.r[0] + b.r[0]) return 0;
        if (Abs(a.c[1] - b.c[1]) > a.r[1] + b.r[1]) return 0;
        if (Abs(a.c[2] - b.c[2]) > a.r[2] + b.r[2]) return 0;
        return 1;
    }
    */

#pragma endregion

#pragma region BoundingVolumeSphere

    // Adjusted for SFML
    struct SphereBV // Had to rename due to conflict with Sphere class
    {
        sf::Vector2f c; // Sphere center
        float r; // Sphere radius
    };
    /* Sphere representation from the book
    // Region R = { (x, y, z) | (x-c.x)∧2 + (y-c.y)∧2 + (z-c.z)∧2 <= r∧2 }
    struct Sphere {
        Point c; // Sphere center
        float r; // Sphere radius
    };
    */

    int TestSphereSphere(SphereBV a, SphereBV b);
    /* Testing Sphere against Sphere from the book
    int TestSphereSphere(Sphere a, Sphere b)
    {
        // Calculate squared distance between centers
        Vector d = a.c - b.c;
        float dist2 = Dot(d, d);
        // Spheres intersect if squared distance is less than squared sum of radii
        float radiusSum = a.r + b.r;
        return dist2 <= radiusSum * radiusSum;
    }
    */

#pragma endregion

    // Intersect sphere s0 moving in direction d over time interval t0 <= t <= t1, against
    // a stationary sphere s1. If found intersecting, return time t of collision
    int TestMovingSphereSphere(SphereBV s0, sf::Vector2<float> d, float t0, float t1, SphereBV s1, float &t);
    /*
    {
        // Compute sphere bounding motion of s0 during time interval from t0 to t1
        Sphere b;
        float mid = (t0 + t1) * 0.5f;
        b.c = s0.c + d * mid;
        b.r = (mid – t0) * Length(d) + s0.r;
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

    // Test collision between objects a and b moving over the time interval
    // [startTime, endTime]. When colliding, time of collision is returned in hitTime
    //int IntervalCollision(Object a, Object b, float startTime, float endTime, float &hitTime);
    /*
    {
        // Compute the maximum distance objects a and b move over the time interval
        float maxMoveA = MaximumObjectMovementOverTime(a, startTime, endTime);
        float maxMoveB = MaximumObjectMovementOverTime(b, startTime, endTime);
        float maxMoveDistSum = maxMoveA + maxMoveB;
        // Exit if distance between a and b at start larger than sum of max movements
        float minDistStart = MinimumObjectDistanceAtTime(a, b, startTime);
        if (minDistStart > maxMoveDistSum) return 0;
        // Exit if distance between a and b at end larger than sum of max movements
        float minDistEnd = MinimumObjectDistanceAtTime(a, b, endTime);
        if (minDistEnd > maxMoveDistSum) return 0;
        // Cannot rule collision out: recurse for more accurate testing. To terminate the
        // recursion, collision is assumed when time interval becomes sufficiently small
        if (endTime – startTime < INTERVAL_EPSILON) {
            hitTime = startTime;
            return 1;
        }
        // Recursively test first half of interval; return collision if detected
        float midTime = (startTime + endTime) * 0.5f;
        if (IntervalCollision(a, b, startTime, midTime, hitTime)) return 1;
        // Recursively test second half of interval
        return IntervalCollision(a, b, midTime, endTime, hitTime);
    }
    */

    // Intersect sphere s with movement vector v with plane p. If intersecting
    // return time t of collision and point q at which sphere hits plane
    int IntersectMovingSpherePlane(collision_detection::Sphere s, sf::Vector2<float> v, collision_detection::Plane p, float &t, sf::Vertex &q);
    /*
    {
        // Compute distance of sphere center to plane
        float dist = Dot(p.n, s.c) - p.d;
        if (Abs(dist) <= s.r) {
            // The sphere is already overlapping the plane. Set time of
            // intersection to zero and q to sphere center
            t = 0.0f;
            q = s.c;
            return 1;
        } else {
            float denom = Dot(p.n, v);
            if (denom * dist >= 0.0f) {
                // No intersection as sphere moving parallel to or away from plane
                return 0;
            } else {
                // Sphere is moving towards the plane
                // Use +r in computations if sphere in front of plane, else -r
                float r = dist > 0.0f ? s.r : -s.r;
                t = (r - dist) / denom;
                q = s.c + t * v – r * p.n;
                return 1;
            }
        }
    }
    */

    // Test if sphere with radius r moving from a to b intersects with plane p
    int TestMovingSpherePlane(sf::Vertex a, sf::Vertex b, float r, collision_detection::Plane p);
    /*
    {
        // Get the distance for both a and b from plane p
        float adist = Dot(a, p.n) - p.d;
        float bdist = Dot(b, p.n) - p.d;
        // Intersects if on different sides of plane (distances have different signs)
        if (adist * bdist < 0.0f) return 1;
        // Intersects if start or end position within radius from plane
        if (Abs(adist) <= r || Abs(bdist) <= r) return 1;
        // No intersection
        return 0;
    }
    */

    int IntersectMovingSphereAABB(collision_detection::Sphere s, sf::Vector2<float> d, sf::FloatRect b, float &t);
    /*
    {
        // Compute the AABB resulting from expanding b by sphere radius r
        AABB e = b;
        e.min.x -= s.r; e.min.y -= s.r; e.min.z -= s.r;
        e.max.x += s.r; e.max.y += s.r; e.max.z += s.r;
        // Intersect ray against expanded AABB e. Exit with no intersection if ray
        // misses e, else get intersection point p and time t as result
        Point p;
        if (!IntersectRayAABB(s.c, d, e, t, p) || t > 1.0f)
            return 0;
        // Compute which min and max faces of b the intersection point p lies
        // outside of. Note, u and v cannot have the same bits set and
        // they must have at least one bit set among them
        int u = 0, v = 0;
        if (p.x < b.min.x) u |= 1;
        if (p.x > b.max.x) v |= 1;
        if (p.y < b.min.y) u |= 2;
        if (p.y > b.max.y) v |= 2;
        if (p.z < b.min.z) u |= 4;
        if (p.z > b.max.z) v |= 4;
        // ‘Or’ all set bits together into a bit mask (note: here u + v == u | v)
        int m = u + v;
        // Define line segment [c, c+d] specified by the sphere movement
        Segment seg(s.c, s.c + d);
        // If all 3 bits set (m == 7) then p is in a vertex region
        if (m == 7) {
            // Must now intersect segment [c, c+d] against the capsules of the three
            // edges meeting at the vertex and return the best time, if one or more hit
            float tmin = FLT_MAX;
            if (IntersectSegmentCapsule(seg, Corner(b, v), Corner(b, v ∧ 1), s.r, &t))
                tmin = Min(t, tmin);
            if (IntersectSegmentCapsule(seg, Corner(b, v), Corner(b, v ∧ 2), s.r, &t))
                tmin = Min(t, tmin);
            if (IntersectSegmentCapsule(seg, Corner(b, v), Corner(b, v ∧ 4), s.r, &t))
                tmin = Min(t, tmin);
            if (tmin == FLT_MAX) return 0; // No intersection
            t = tmin;
            return 1; // Intersection at time t == tmin
        }
        // If only one bit set in m, then p is in a face region
        if ((m & (m - 1)) == 0) {
            // Do nothing. Time t from intersection with
            // expanded box is correct intersection time
            return 1;
        }
        // p is in an edge region. Intersect against the capsule at the edge
        return IntersectSegmentCapsule(seg, Corner(b, u ∧ 7), Corner(b, v), s.r, &t);
    }
    */

    // Support function that returns the AABB vertex with index n
    //Point Corner(Rect b, int n);
    /*
    {
        sf::Vertex p;
        p.x = ((n & 1) ? b.max.x : b.min.x);
        p.y = ((n & 2) ? b.max.y : b.min.y);
        p.z = ((n & 4) ? b.max.z : b.min.z);
        return p;
    }
    */

} // collision_detection

#endif //INTERSECTIONTESTS_H
