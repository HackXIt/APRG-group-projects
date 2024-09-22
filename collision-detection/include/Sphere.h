//
// Created by RINI on 22/09/2024.
//

#ifndef SPHERE_H
#define SPHERE_H
#include "BoundingVolume.h"
#include "ei/2dtypes.hpp"

namespace collision_detection {

    class Sphere : public BoundingVolume {
    public:
        ei::Vec2 center;
        float radius;
        ei::Disc2D disc;
        sf::CircleShape debugShape;

        Sphere(sf::Shape* shape) : BoundingVolume(shape), center(0.0f, 0.0f), radius(0.0f), disc(center, radius)
        {
            debugShape = sf::CircleShape(radius);
            debugShape.setOrigin(radius, radius);
            debugShape.setPosition(center.x, center.y);
            debugShape.setFillColor(sf::Color::Transparent);
            debugShape.setOutlineColor(sf::Color::Yellow);
            debugShape.setOutlineThickness(1.0f);
            debugShape.setFillColor(sf::Color::Transparent);
        }

        void calculateFromShape() override;
        bool intersects(const BoundingVolume& other) const override;
        void draw(sf::RenderWindow& window) const override;

        std::string toString() const override;
    };
} // collision_detection

#endif //SPHERE_H
