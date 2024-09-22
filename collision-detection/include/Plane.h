//
// Created by RINI on 22/09/2024.
//

#ifndef PLANE_H
#define PLANE_H
#include <ei/2dtypes.hpp>

#include "BoundingVolume.h"

namespace collision_detection {

    class Plane : public BoundingVolume {
        public:
        ei::Segment2D line;
        sf::VertexArray debugShape;

        Plane(sf::Vertex points[])
        {
            line = ei::Segment2D(ei::Vec2(points[0].position.x, points[0].position.y), ei::Vec2(points[0].position.x, points[0].position.y));
            debugShape = sf::VertexArray(sf::LineStrip, 2);
            for(int i = 0; i < 2; i++)
            {
                debugShape.append(points[i]);
            }
        }

        Plane(sf::Shape* shape) : BoundingVolume(shape)
        {
            line = ei::Segment2D(ei::Vec2(0.0f, 0.0f), ei::Vec2(0.0f, 0.0f));
            debugShape = sf::VertexArray(sf::LinesStrip, 2);
            debugShape.append(sf::Vertex(sf::Vector2f(0.0f, 0.0f)));
            debugShape.append(sf::Vertex(sf::Vector2f(0.0f, 0.0f)));
            for(int i = 0; i < 2; i++)
            {
                debugShape[i].color = sf::Color::Yellow;
            }
        }

        bool intersects(const BoundingVolume& other) const override;
        void calculateFromShape() override;
        void draw(sf::RenderWindow& window) const override;

        std::string toString() const override;
    };

} // collision_detection

#endif //PLANE_H
