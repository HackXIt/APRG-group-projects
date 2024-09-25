//
// Created by RINI on 24/09/2024.
//

#ifndef VISUALVECTOR_H
#define VISUALVECTOR_H
#include "ei/2dtypes.hpp"
#include "SFML/Graphics.hpp"


class VisualVector {
public:
    ei::Vec2 start;
    ei::Vec2 end;
    sf::VertexArray line;

    VisualVector(ei::Vec2 start, ei::Vec2 end) : start(start), end(end)
    {
        line = sf::VertexArray(sf::LinesStrip, 2);
        line.append(sf::Vertex(sf::Vector2f(start.x, start.y), sf::Color::Blue));
        line.append(sf::Vertex(sf::Vector2f(end.x, end.y), sf::Color::Blue));
    }

    void draw(sf::RenderWindow& window);
};



#endif //VISUALVECTOR_H
