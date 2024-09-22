//
// Created by RINI on 22/09/2024.
//

#ifndef BOUNDINGVOLUME_H
#define BOUNDINGVOLUME_H

#include <SFML/Graphics.hpp>

namespace collision_detection {

class BoundingVolume {
    // Virtual destructor for proper cleanup of derived classes
    virtual ~BoundingVolume() {}

    // Pure virtual function for intersection test with another bounding volume
    virtual bool intersects(const BoundingVolume& other) const = 0;

    // Function to calculate the bounding volume from an SFML shape
    virtual void calculateFromShape(const sf::Shape& shape) = 0;

    // Optional: Function to visualize the bounding volume (for debugging)
    virtual void draw(sf::RenderWindow& window) const = 0;
};

} // collision_detection

#endif //BOUNDINGVOLUME_H
