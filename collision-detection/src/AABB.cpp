//
// Created by RINI on 22/09/2024.
//

#include "AABB.h"

namespace collision_detection {
    void AABB::calculateFromShape(const sf::Shape& shape) {
        bounds = shape.getGlobalBounds();
        // Update the debug shape
        debugShape.setSize(sf::Vector2f(bounds.width, bounds.height));
        debugShape.setPosition(bounds.left, bounds.top);
    }

    bool AABB::intersects(const BoundingVolume& other) const {
        const AABB* otherAABB = dynamic_cast<const AABB*>(&other);
        if (otherAABB) {
            return bounds.intersects(otherAABB->bounds);
        }
        // Handle other types similarly
        return false;
    }

    void AABB::draw(sf::RenderWindow& window) const {
        window.draw(debugShape);
    };
} // collision_detection