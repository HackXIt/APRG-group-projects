//
// Created by RINI on 22/09/2024.
//

#include "Sphere.h"

#include <cmath>

namespace collision_detection {
    void Sphere::calculateFromShape(const sf::Shape& shape) {
        // Compute the centroid and the maximum distance to any point (radius)
        sf::FloatRect bounds = shape.getGlobalBounds();
        center = sf::Vector2f(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
        radius = std::max(bounds.width, bounds.height) / 2;
        // Update the debug shape
        debugShape.setRadius(radius);
        debugShape.setOrigin(radius, radius);
        debugShape.setPosition(center);
    }

    bool Sphere::intersects(const BoundingVolume& other) const {
        // Implement intersection logic with another bounding volume
        // For simplicity, let's assume 'other' is also a Sphere
        const Sphere* otherSphere = dynamic_cast<const Sphere*>(&other);
        if (otherSphere) {
            float distance = sqrt(std::pow(center.x - otherSphere->center.x, 2) + pow(center.y - otherSphere->center.y, 2));
            return distance <= (radius + otherSphere->radius);
        }
        // Handle other types similarly
        return false;
    }

    void Sphere::draw(sf::RenderWindow& window) const {
        window.draw(debugShape);
    }
} // dynamic_intersection