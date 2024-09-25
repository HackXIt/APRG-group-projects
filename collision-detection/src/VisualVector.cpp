//
// Created by RINI on 24/09/2024.
//

#include "VisualVector.h"

#include <iostream>

#include "app.h"

void VisualVector::draw(sf::RenderWindow& window)
{
    line[0].position = sf::Vector2f(start.x, start.y);
    line[1].position = sf::Vector2f(end.x, end.y);

    // std::cout << "Drawing line from (" << start.x << ", " << start.y << ") to (" << end.x << ", " << end.y << ")" << std::endl;

    window.draw(line);
}
