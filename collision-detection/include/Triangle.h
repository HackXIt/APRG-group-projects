//
// Created by RINI on 22/09/2024.
//

#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "BoundingVolume.h"

namespace collision_detection {

    class Triangle : public BoundingVolume {
    public:
        sf::Vector2f points[3];
        sf::ConvexShape debugShape;

        Triangle()
        {
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
