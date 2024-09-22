//
// Created by RINI on 22/09/2024.
//

#ifndef BOUNDINGVOLUME_H
#define BOUNDINGVOLUME_H

#include <SFML/Graphics.hpp>

namespace collision_detection {

class BoundingVolume {
    // Virtual destructor for proper cleanup of derived classes
public:
    BoundingVolume() : isColliding(false), boundedShape(nullptr) {}
    explicit BoundingVolume(sf::Shape* shape) : isColliding(false), boundedShape(shape) {}
    virtual ~BoundingVolume() = default;
    bool isColliding;
    sf::Shape* boundedShape;
    // Pure virtual function for intersection test with another bounding volume
    virtual bool intersects(const BoundingVolume& other) const = 0;

    // Function to calculate the bounding volume from an SFML shape
    virtual void calculateFromShape() = 0;
    void calculateFromShape(sf::Shape* shape) {
        boundedShape = shape;
        calculateFromShape();
    }

    // Optional: Function to visualize the bounding volume (for debugging)
    virtual void draw(sf::RenderWindow& window) const = 0;

    virtual std::string toString() const = 0;
};

} // collision_detection

#endif //BOUNDINGVOLUME_H
