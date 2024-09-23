//
// Created by RINI on 23/09/2024.
//

#ifndef LINEARMOTION_H
#define LINEARMOTION_H
#include "BoundingVolume.h"
#include "SFML/Graphics/Shape.hpp"
#include "ei/2dtypes.hpp"

class LinearMotion {
public:
    LinearMotion(sf::Shape& shape, float velocity = 0.0f, sf::Vector2f target = {0.0f, 0.0f});
    void update();

    void setVelocity(float v);
    float getVelocity() const;

    ei::Vec2 getVelocityVec2() const;

    void setTarget(sf::Vector2f t);
    sf::Vector2f getTarget() const;

    void moveShape(float x, float y);

    sf::Vector2f getShapePosition() const;

    /**
     * @brief Method to get position at time t without altering current state
     * @param t Time value to get position at.
     */
    sf::Vector2f getPositionAtTime(float t) const;

    /**
     * @brief Method to get position at time t without altering current state
     * @param t Time value to get position at.
     */
    ei::Vec2 getPositionAtTimeEi(float t) const;

    float findCollisionTime(float t0, float t1, float tolerance, collision_detection::BoundingVolume& movingObject, const collision_detection::BoundingVolume& staticObject) const;

private:
    sf::Shape& shapeToMove;
    float velocity;
    sf::Vector2f target;
};



#endif //LINEARMOTION_H
