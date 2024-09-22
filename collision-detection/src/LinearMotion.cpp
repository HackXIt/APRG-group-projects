//
// Created by RINI on 23/09/2024.
//

#include <cmath>
#include "LinearMotion.hpp"

LinearMotion::LinearMotion(sf::Shape& shape, float velocity, sf::Vector2f target)
    : shapeToMove(shape), velocity(velocity), target(target)
{
}

void LinearMotion::update()
{
    if(shapeToMove.getPosition() == target)
    {
        return;
    }
    // Get the current position of the shape
    sf::Vector2f currentPosition = shapeToMove.getPosition();

    // Calculate the direction vector from the current position to the target
    sf::Vector2f direction = target - currentPosition;

    // Calculate the distance to the target
    float distance = sqrtf(direction.x * direction.x + direction.y * direction.y);

    // If the distance is greater than a small threshold (to prevent jittering when close to the target)
    if (distance > 0.01f)
    {
        // Normalize the direction vector
        sf::Vector2f normalizedDirection = direction / distance;

        // Move the shape towards the target based on the velocity and time step
        sf::Vector2f velocityVector = normalizedDirection * velocity;

        // Update the shape's position (assuming you're using deltaTime for time-based movement)
        shapeToMove.setPosition(currentPosition + velocityVector);
    }
}

void LinearMotion::setVelocity(float v)
{
    velocity = v;
}

float LinearMotion::getVelocity() const
{
    return velocity;
}

void LinearMotion::setTarget(sf::Vector2f t)
{
    target = t;
}

sf::Vector2f LinearMotion::getTarget() const
{
    return target;
}