//
// Created by RINI on 23/09/2024.
//

#include <cmath>
#include "LinearMotion.hpp"

LinearMotion::LinearMotion(sf::Shape& shape, float velocity, sf::Vector2f target)
    : shapeToMove(shape), velocity(velocity), target(target)
{
}

void LinearMotion::update() const
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

void LinearMotion::moveShape(float x, float y)
{
    target = shapeToMove.getPosition() + sf::Vector2f(x, y);
    update();
}

sf::Vector2f LinearMotion::getShapePosition() const
{
    return shapeToMove.getPosition();
}

sf::Vector2f LinearMotion::getPositionAtTime(float t) const
{
    // Calculate the position of the shape at time t
    sf::Vector2f currentPosition = shapeToMove.getPosition();
    sf::Vector2f direction = target - currentPosition;
    sf::Vector2f normalizedDirection = direction / sqrtf(direction.x * direction.x + direction.y * direction.y);
    sf::Vector2f velocityVector = normalizedDirection * velocity;
    return currentPosition + velocityVector * t;
}

ei::Vec2 LinearMotion::getPositionAtTimeEi(float t) const
{
    // Calculate the position of the shape at time t
    sf::Vector2f currentPosition = shapeToMove.getPosition();
    sf::Vector2f direction = target - currentPosition;
    sf::Vector2f normalizedDirection = direction / sqrtf(direction.x * direction.x + direction.y * direction.y);
    sf::Vector2f velocityVector = normalizedDirection * velocity;
    return {currentPosition.x + velocityVector.x * t, currentPosition.y + velocityVector.y * t};
}

float LinearMotion::findCollisionTime(float t0, float t1, float tolerance, collision_detection::BoundingVolume& movingObject, const collision_detection::BoundingVolume& staticObject) const
{
    float collisionTime = -1.0f;
    sf::Vector2f currentPosition = movingObject.boundedShape->getPosition();
    while (t1 - t0 > tolerance) {
        float tm = (t0 + t1) / 2.0f;

        // Get position of the moving object at time tm
        sf::Vector2f positionAtTm = getPositionAtTime(tm);

        // Temporarily set the position of the moving object
        movingObject.boundedShape->setPosition(positionAtTm);
        movingObject.calculateFromShape();

        // Check for collision
        if (movingObject.intersects(staticObject)) {
            collisionTime = tm;
            t1 = tm; // Collision detected, search earlier times
        } else {
            t0 = tm; // No collision, search later times
        }
    }

    // Restore the original position of the moving object
    movingObject.boundedShape->setPosition(currentPosition);
    movingObject.calculateFromShape();

    return collisionTime;
}

void LinearMotion::setVelocity(float v)
{
    velocity = v;
}

float LinearMotion::getVelocity() const
{
    return velocity;
}

ei::Vec2 LinearMotion::getVelocityVec2() const
{
    return {velocity, velocity};
}


void LinearMotion::setTarget(sf::Vector2f t)
{
    target = t;
}

sf::Vector2f LinearMotion::getTarget() const
{
    return target;
}