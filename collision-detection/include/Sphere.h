//
// Created by RINI on 22/09/2024.
//

#ifndef SPHERE_H
#define SPHERE_H
#include "BoundingVolume.h"

namespace collision_detection {

    class Sphere : public BoundingVolume {
    public:
        sf::Vector2f center;
        float radius;
        sf::CircleShape debugShape;

        Sphere() : center(0, 0), radius(0)
        {
            debugShape = sf::CircleShape(radius);
            debugShape.setOrigin(radius, radius);
            debugShape.setPosition(center);
            debugShape.setFillColor(sf::Color::Transparent);
            debugShape.setOutlineColor(sf::Color::Yellow);
            debugShape.setOutlineThickness(1.0f);
            debugShape.setFillColor(sf::Color::Transparent);
        }

        void calculateFromShape(const sf::Shape& shape) override;
        bool intersects(const BoundingVolume& other) const override;
        void draw(sf::RenderWindow& window) const override;

    };
} // collision_detection

#endif //SPHERE_H
