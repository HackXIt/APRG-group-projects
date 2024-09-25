//
// Created by RINI on 22/09/2024.
//

#include "Sphere.h"

#include <cfloat>

#include "ei/2dintersection.hpp"

#include <cmath>
#include <iostream>

#include "AABB.h"
#include "app.h"
#include "Plane.h"
#include "Triangle.h"
#include "VisualVector.h"

namespace collision_detection {

    void Sphere::calculateFromThis()
    {
        debugShape.setRadius(radius);
        debugShape.setOrigin(radius, radius); // Center the circle shape on its position

        // Adjust positions to SFML coordinate system
        float windowCenterX = WINDOW_DEFAULT_WIDTH / 2.f;
        float windowCenterY = WINDOW_DEFAULT_HEIGHT / 2.f;

        // Adjust the position to SFML's coordinate system
        debugShape.setPosition(center.x + windowCenterX, windowCenterY - center.y);
    }

    void Sphere::calculateFromShape()
    {
        // Compute the centroid and the maximum distance to any point (radius)
        sf::FloatRect globalBounds = boundedShape->getGlobalBounds();
        center = ei::Vec2(globalBounds.left + globalBounds.width / 2, globalBounds.top + globalBounds.height / 2);
        radius = std::max(globalBounds.width, globalBounds.height) / 2;
        disc.center = center;
        disc.radius = radius;
        // Update the debug shape
        debugShape.setRadius(radius);
        debugShape.setOrigin(radius, radius);
        debugShape.setPosition(center.x, center.y);
    }

    bool Sphere::intersects(const BoundingVolume& other) const
    {
        // Based on otherType, call the appropriate intersection test
        const Sphere* otherSphere = dynamic_cast<const Sphere*>(&other);
        if (otherSphere)
        {
            float t0 = 0.0f;
            // return ei::intersects(disc, otherSphere->disc);
            return testMovingSphereSphere(*this, {0.0f, 0.0f}, 0.0f, 1.0f, *otherSphere, t0);
        }
        const AABB* otherAABB = dynamic_cast<const AABB*>(&other);
        if (otherAABB)
        {
            return ei::intersects(disc, otherAABB->bounds);
        }
        const Plane* otherPlane = dynamic_cast<const Plane*>(&other);
        if (otherPlane)
        {
            return ei::intersects(disc, otherPlane->line.a) || ei::intersects(disc, otherPlane->line.b);
        }
        const Triangle* otherTriangle = dynamic_cast<const Triangle*>(&other);
        if(otherTriangle)
        {
            // There's no intersection test for triangle :O
            return false;
        }
        // No derived type matched
        return false;
    }

    void Sphere::draw(sf::RenderWindow& window) const {
        drawBounded(window);
        drawDebug(window);
    }

    void Sphere::drawBounded(sf::RenderWindow& window) const
    {
        if(isColliding)
        {
            boundedShape->setFillColor(sf::Color::Green);
        } else
        {
            boundedShape->setFillColor(sf::Color::Transparent);
        }
        window.draw(*boundedShape);
    }


    void Sphere::drawDebug(sf::RenderWindow& window) const
    {
        window.draw(debugShape);
    }


    std::string Sphere::toString() const {
        std::string result = "Sphere:\ncenter = " + formatFloat(center.x, 0) + ":" + formatFloat(center.y, 0)
            + "\nradius = " + formatFloat(radius, 0)
            + "\ncollided = " + (isColliding ? "true" : "false");
        return result;
    }

    bool Sphere::testMovingSphereSphereVisualContinous(const ei::Vec2& d, float t0, float t1, const Sphere& s1, float& collisionTime, sf::RenderWindow& window)
    {
        constexpr float INTERVAL_EPSILON = 0.001f; // Precision for time intervals
        float currentTime = t0;

        // Loop continuously until we detect a collision or exceed the time limit
        while (currentTime < t1)
        {
            // Compute the current midpoint
            float mid = (currentTime + t1) * 0.5f;

            // Compute the center of the bounding sphere at time 'mid'
            ei::Vec2 s0_c = disc.center;
            ei::Vec2 b_c = s0_c + d * mid; // Calculate the position at time 'mid'

            // Compute the bounding sphere's center and radius
            Sphere b;
            b.disc.center = b_c;

            // Compute the radius of the bounding sphere
            float movementDistance = (mid - t0) * sqrt(d.x * d.x + d.y * d.y);
            b.disc.radius = movementDistance + disc.radius;

            // Check for collision with the target sphere
            if (ei::intersects(b.disc, s1.disc))
            {
                // Collision detected
                collisionTime = currentTime; // Mark the time of collision
                return true;
            }

            // Draw the current state: Moving sphere and its bounding sphere
            window.clear(sf::Color::Black); // Clear window

            // Draw the original moving sphere
            draw(window); // This draws the moving sphere (your current object)

            // Draw the bounding sphere for this motion step
            b.calculateFromThis();
            b.drawDebug(window); // This draws the bounding sphere

            // Draw the target sphere
            s1.draw(window); // Draw the target sphere

            // Display the current frame
            window.display();

            // Progress the time
            currentTime += INTERVAL_EPSILON; // Increment the time step (fine-tuned for precision)

            // Handle SFML events (e.g., window close)
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }
        }

    // If we exit the loop, it means no collision occurred within the interval
    return false;
}

    bool Sphere::testMovingSphereSphereVisual(const ei::Vec2& d, float t0, float t1, const Sphere& s1, float& collisionTime, sf::RenderWindow& window)
    {
        // Compute sphere (circle) bounding motion of s0 during time interval from t0 to t1
        Sphere b;
        float mid = (t0 + t1) * 0.5f;

        // Compute the center of the bounding sphere at time 'mid'
        ei::Vec2 s0_c = disc.center;
        ei::Vec2 b_c = s0_c + d * mid;

        b.disc.center = b_c;

        // Compute the radius of the bounding sphere
        float movementDistance = (mid - t0) * sqrt(d.x * d.x + d.y * d.y);
        b.disc.radius = movementDistance + disc.radius;


        // std::cout << "s0_c: (" << s0_c.x << ", " << s0_c.y << ")" << std::endl;
        // std::cout << "b_c: (" << b_c.x << ", " << b_c.y << ")" << std::endl;
        // std::cout << "d: (" << d.x << ", " << d.y << ")" << std::endl;

        // If bounding sphere not overlapping s1, then no collision in this interval
        if (!ei::intersects(b.disc, s1.disc))
        {
            return false;
        }

        // Cannot rule collision out: recurse for more accurate testing
        constexpr float INTERVAL_EPSILON = 0.001f; // Adjust for desired precision
        if (t1 - t0 < INTERVAL_EPSILON) {
            collisionTime = t0;
            return true;
        }

        // Recursively test first half of interval; return collision if detected
        if (testMovingSphereSphereVisual(d, t0, mid, s1, collisionTime, window))
        {
            return true;
        }

        // Recursively test second half of interval
        return testMovingSphereSphereVisual(d, mid, t1, s1, collisionTime, window);
    }


    bool testMovingSphereSphere(
        const Sphere& s0,
        const ei::Vec2& d,
        float t0, float t1,
        const Sphere& s1,
        float& collisionTime)
    {
        // Compute sphere (circle) bounding motion of s0 during time interval from t0 to t1
        Sphere b;
        float mid = (t0 + t1) * 0.5f;

        // Compute the center of the bounding sphere at time 'mid'
        ei::Vec2 s0_c = s0.disc.center;
        ei::Vec2 b_c = s0_c + d * mid;
        b.disc.center = b_c;

        // Compute the radius of the bounding sphere
        float movementDistance = (mid - t0) * sqrt(d.x * d.x + d.y * d.y);
        b.disc.radius = movementDistance + s0.disc.radius;

        // If bounding sphere not overlapping s1, then no collision in this interval
        if (!intersects(b.disc, s1.disc))
            return false;

        // Cannot rule collision out: recurse for more accurate testing
        const float INTERVAL_EPSILON = 0.001f; // Adjust for desired precision
        if (t1 - t0 < INTERVAL_EPSILON) {
            collisionTime = t0;
            return true;
        }

        // Recursively test first half of interval; return collision if detected
        if (testMovingSphereSphere(s0, d, t0, mid, s1, collisionTime))
            return true;

        // Recursively test second half of interval
        return testMovingSphereSphere(s0, d, mid, t1, s1, collisionTime);
    }
/*
    bool IntersectMovingSphereAABB(
        const collision_detection::Sphere& s,
        const ei::Vec2& d,
        const collision_detection::AABB& b,
        float& collisionTime)
    {
        // Compute the AABB resulting from expanding b by sphere radius r
        AABB e = b;
        e.bounds.min.x -= s.radius; e.bounds.min.y -= s.radius; // e.bounds.min.z -= s.r;
        e.bounds.max.x += s.radius; e.bounds.max.y += s.radius; // e.bounds.max.z += s.radius;
        // Intersect ray against expanded AABB e. Exit with no intersection if ray
        // misses e, else get intersection point p and time t as result
        Point p;
        if (!IntersectRayAABB(s.c, d, e, t, p) || t > 1.0f)
            return 0;
        // Compute which min and max faces of b the intersection point p lies
        // outside of. Note, u and v cannot have the same bits set and
        // they must have at least one bit set among them
        int u = 0, v = 0;
        if (p.x < b.bounds.min.x) u |= 1;
        if (p.x > b.bounds.max.x) v |= 1;
        if (p.y < b.bounds.min.y) u |= 2;
        if (p.y > b.bounds.max.y) v |= 2;
        // if (p.z < b.bounds.min.z) u |= 4;
        // if (p.z > b.bounds.max.z) v |= 4;
        // ‘Or’ all set bits together into a bit mask (note: here u + v == u | v)
        int m = u + v;
        // Define line segment [c, c+d] specified by the sphere movement
        ei::Segment2D seg(s.center, s.center + d);
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
} // dynamic_intersection