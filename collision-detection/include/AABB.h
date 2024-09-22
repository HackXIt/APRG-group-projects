//
// Created by RINI on 22/09/2024.
//

#ifndef AABB_H
#define AABB_H
#include <ei/2dtypes.hpp>

#include "BoundingVolume.h"

namespace collision_detection {

    class AABB : public BoundingVolume {
    public:
        ei::Rect2D bounds;
        sf::RectangleShape debugShape;

        AABB(sf::Shape* shape) : BoundingVolume(shape)
        {
            bounds = ei::Rect2D(ei::Vec2(0.0f, 0.0f), ei::Vec2(0.0f, 0.0f));
            debugShape = sf::RectangleShape(sf::Vector2f(bounds.min.x, bounds.max.y));
            debugShape.setPosition(bounds.min.x, bounds.min.y);
            debugShape.setFillColor(sf::Color::Transparent);
            debugShape.setOutlineColor(sf::Color::Yellow);
            debugShape.setOutlineThickness(1.0f);
        }

        bool intersects(const BoundingVolume& other) const override;
        void calculateFromShape() override;
        void draw(sf::RenderWindow& window) const override;

        std::string toString() const override;
    };

} // collision_detection

#endif //AABB_H
