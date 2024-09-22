//
// Created by RINI on 22/09/2024.
//

#include "AABB.h"

#include <ei/2dintersection.hpp>

#include "Sphere.h"
#include "Triangle.h"

namespace collision_detection {
    void AABB::calculateFromShape(const sf::Shape& shape) {
        sf::FloatRect globalBounds = shape.getGlobalBounds(); // Global bounds is not minimal, might cause issues!!
        bounds.min.x = globalBounds.left;
        bounds.min.y = globalBounds.top;
        bounds.max.x = globalBounds.left + globalBounds.width;
        bounds.max.y = globalBounds.top + globalBounds.height;
        // Update the debug shape
        debugShape.setSize(globalBounds.getSize());
        debugShape.setPosition(globalBounds.left, globalBounds.top);
    }

    bool AABB::intersects(const BoundingVolume& other) const {
        // Based on otherType, call the appropriate intersection test
        const Sphere* otherSphere = dynamic_cast<const Sphere*>(&other);
        if (otherSphere)
        {
            return ei::intersects(bounds, otherSphere->disc);
        }
        const AABB* otherAABB = dynamic_cast<const AABB*>(&other);
        if (otherAABB)
        {
            return ei::intersects(bounds, otherAABB->bounds);
        }
        const Triangle* otherTriangle = dynamic_cast<const Triangle*>(&other);
        if(otherTriangle)
        {
            // No intersection test for triangle (not implemented)
            return false;
        }
        // No derived type matched
        return false;
    }

    void AABB::draw(sf::RenderWindow& window) const {
        window.draw(debugShape);
    };
} // collision_detection