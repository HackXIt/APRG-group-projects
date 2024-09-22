//
// Created by RINI on 22/09/2024.
//

#include "Triangle.h"

#include <ei/2dintersection.hpp>

#include "AABB.h"
#include "app.h"
#include "Sphere.h"

namespace collision_detection {
    void Triangle::calculateFromShape() {
        // FIXME I think this needs some work
        // Assume the shape has at least 3 points
        for (int i = 0; i < 3; ++i) {
            points[i] = boundedShape->getTransform().transformPoint(boundedShape->getPoint(i));
        }
        // Update the debug shape
        for (int i = 0; i < 3; ++i) {
            debugShape.setPoint(i, points[i]);
        }
    }

    bool Triangle::intersects(const BoundingVolume& other) const {
        // No intersection test for triangle (not implemented yet)
        return false;
    }

    void Triangle::draw(sf::RenderWindow& window) const {
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

    std::string Triangle::toString() const {
        std::string result = "Triangle:\nA = " + formatFloat(points[0].x, 0) + ":" + formatFloat(points[0].y, 0)
            + "\nB = " + formatFloat(points[1].x, 0) + ":" + formatFloat(points[1].y, 0)
            + "\nC = " + formatFloat(points[2].x, 0) + ":" + formatFloat(points[2].y, 0)
            + "\nCollided = " + (isColliding ? "true" : "false");
        return result;
    }
} // collision_detection