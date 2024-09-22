//
// Created by RINI on 22/09/2024.
//

#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <ei/2dtypes.hpp>

#include "BoundingVolume.h"

namespace collision_detection {

    class Triangle : public BoundingVolume {
    public:
        ei::Triangle2D triangle;
        sf::Vector2f points[3];
        sf::ConvexShape debugShape;

        Triangle()
        {
            points[0] = sf::Vector2f(0.0f, 0.0f);
            points[1] = sf::Vector2f(0.0f, 0.0f);
            points[2] = sf::Vector2f(0.0f, 0.0f);
            triangle = ei::Triangle2D(ei::Vec2(points[0].x, points[0].y), ei::Vec2(points[1].x, points[1].y), ei::Vec2(points[2].x, points[2].y));
            debugShape = sf::ConvexShape(3);
            for (int i = 0; i < 3; ++i) {
                debugShape.setPoint(i, points[i]);
            }
            debugShape.setFillColor(sf::Color::Transparent);
            debugShape.setOutlineColor(sf::Color::Yellow);
            debugShape.setOutlineThickness(1.0f);
        }

        void calculateFromShape(const sf::Shape& shape) override;
        bool intersects(const BoundingVolume& other) const override;
        void draw(sf::RenderWindow& window) const override;
    };

} // collision_detection

#endif //TRIANGLE_H
