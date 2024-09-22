//
// Created by RINI on 22/09/2024.
//

#include "Plane.h"

#include <ei/2dintersection.hpp>

#include "AABB.h"
#include "app.h"
#include "Sphere.h"
#include "Triangle.h"

namespace collision_detection {
    void Plane::calculateFromShape() {
        // Update the debug shape
        debugShape[0].position.x = line.a.x;
        debugShape[0].position.y = line.a.y;
        debugShape[1].position.x = line.b.x;
        debugShape[1].position.y = line.b.y;
    }

    bool Plane::intersects(const BoundingVolume& other) const {
        // Based on otherType, call the appropriate intersection test
        const Sphere* otherSphere = dynamic_cast<const Sphere*>(&other);
        if (otherSphere)
        {
            return ei::intersects(line.a, otherSphere->disc) || ei::intersects(line.b, otherSphere->disc);
        }
        const AABB* otherAABB = dynamic_cast<const AABB*>(&other);
        if (otherAABB)
        {
            return ei::intersects(line.a, otherAABB->bounds) || ei::intersects(line.b, otherAABB->bounds);
        }
        const Plane* otherPlane = dynamic_cast<const Plane*>(&other);
        if (otherPlane)
        {
            // No intersection test for Plane-Plane (not implemented)
            return false;
        }
        const Triangle* otherTriangle = dynamic_cast<const Triangle*>(&other);
        if(otherTriangle)
        {
            // No intersection test for Plane-Triangle (not implemented)
            return false;
        }
        // No derived type matched
        return false;
    }

    void Plane::draw(sf::RenderWindow& window) const {
        // Unable to change the color of a line
        window.draw(debugShape); // Nothing else to draw
    };

    std::string Plane::toString() const {
        std::string result = "Plane:\nA = " + formatFloat(line.a.x, 0) + ":" + formatFloat(line.a.y, 0)
            + "\nB = " + formatFloat(line.b.x, 0) + ":" + formatFloat(line.b.y, 0)
            + "\nCollided = " + (isColliding ? "true" : "false");
        return result;
    }
} // collision_detection