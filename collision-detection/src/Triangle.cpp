//
// Created by RINI on 22/09/2024.
//

#include "Triangle.h"

namespace collision_detection {
    void Triangle::calculateFromShape(const sf::Shape& shape) {
        // Assume the shape has at least 3 points
        for (int i = 0; i < 3; ++i) {
            points[i] = shape.getTransform().transformPoint(shape.getPoint(i));
        }
        // Update the debug shape
        for (int i = 0; i < 3; ++i) {
            debugShape.setPoint(i, points[i]);
        }
    }

    bool Triangle::intersects(const BoundingVolume& other) const {
        // Implement triangle intersection tests
        return false; // Placeholder
    }

    void Triangle::draw(sf::RenderWindow& window) const {
        window.draw(debugShape);
    }
} // collision_detection