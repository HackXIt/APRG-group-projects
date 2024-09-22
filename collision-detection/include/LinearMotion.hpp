//
// Created by RINI on 23/09/2024.
//

#ifndef LINEARMOTION_H
#define LINEARMOTION_H
#include "SFML/Graphics/Shape.hpp"


class LinearMotion {
public:
    LinearMotion(sf::Shape& shape, float velocity = 0.0f, sf::Vector2f target = {0.0f, 0.0f});
    void update();

    void setVelocity(float v);
    float getVelocity() const;

    void setTarget(sf::Vector2f t);
    sf::Vector2f getTarget() const;

private:
    sf::Shape& shapeToMove;
    float velocity;
    sf::Vector2f target;
};



#endif //LINEARMOTION_H
