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
            return ei::intersects(disc, otherSphere->disc);
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