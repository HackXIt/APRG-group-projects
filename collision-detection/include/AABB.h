//
// Created by RINI on 22/09/2024.
//

#ifndef AABB_H
#define AABB_H
#include "BoundingVolume.h"

namespace collision_detection {

    class AABB : public BoundingVolume {
    public:
        sf::FloatRect bounds;
        sf::RectangleShape debugShape;

        AABB() : bounds(0, 0, 0, 0)
        {
            debugShape = sf::RectangleShape(sf::Vector2f(bounds.width, bounds.height));
            debugShape.setPosition(bounds.left, bounds.top);
            debugShape.setFillColor(sf::Color::Transparent);
            debugShape.setOutlineColor(sf::Color::Yellow);
            debugShape.setOutlineThickness(1.0f);
        }

        bool intersects(const BoundingVolume& other) const override;
        void calculateFromShape(const sf::Shape& shape) override;
        void draw(sf::RenderWindow& window) const override;
    };

} // collision_detection

#endif //AABB_H
