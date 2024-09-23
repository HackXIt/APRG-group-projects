//
// Created by RINI on 22/09/2024.
//

#include "Sphere.h"
#include "ei/2dintersection.hpp"

#include <cmath>

#include "AABB.h"
#include "app.h"
#include "Plane.h"
#include "Triangle.h"

namespace collision_detection {
    bool testMovingSphereSphere(
        const collision_detection::Sphere& s0,
        const ei::Vec2& d,
        float t0,
        float t1,
        const collision_detection::Sphere& s1,
        float& collisionTime)
    {
        // Compute sphere (circle) bounding motion of s0 during time interval from t0 to t1
        collision_detection::Sphere b;
        float mid = (t0 + t1) * 0.5f;

        // Compute the center of the bounding sphere at time 'mid'
        ei::Vec2 s0_c = s0.disc.center;
        ei::Vec2 b_c = s0_c + d * mid;
        b.disc.center = b_c;

        // Compute the radius of the bounding sphere
        float movementDistance = (mid - t0) * sqrt(d.x * d.x + d.y * d.y);
        b.disc.radius = movementDistance + s0.disc.radius;

        // If bounding sphere not overlapping s1, then no collision in this interval
        if (!ei::intersects(b.disc, s1.disc))
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
        if(isColliding)
        {
            boundedShape->setFillColor(sf::Color::Green);
        } else
        {
            boundedShape->setFillColor(sf::Color::Red);
        }
        window.draw(*boundedShape);
        window.draw(debugShape);
    }

    std::string Sphere::toString() const {
        std::string result = "Sphere:\ncenter = " + formatFloat(center.x, 0) + ":" + formatFloat(center.y, 0)
            + "\nradius = " + formatFloat(radius, 0)
            + "\ncollided = " + (isColliding ? "true" : "false");
        return result;
    }
} // dynamic_intersection