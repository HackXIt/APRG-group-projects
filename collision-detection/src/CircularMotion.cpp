//
// Created by RINI on 20/09/2024.
//

// CircularMotion.cpp

#include "CircularMotion.hpp"
#include <cmath>

#include "SFML/Graphics/Shape.hpp"

CircularMotion::CircularMotion(sf::Shape& shape, float radius, sf::Vector2f center, float angle, float angularSpeed, float angularSpeedIncrement, bool isClockwise)
    : shapeToRotate(shape), radius(radius), center(center), angle(angle), angularSpeed(angularSpeed), angularSpeedIncrement(angularSpeedIncrement), isClockwise(isClockwise)
{
}

void CircularMotion::update()
{
    // Update the position of the shape in a circular motion
    float x = center.x + radius * std::cos(angle);
    float y = center.y + radius * std::sin(angle);
    shapeToRotate.setPosition(x, y);

    // Increment the angle to keep the shape moving
    if (isClockwise)
        angle += angularSpeed;
    else
        angle -= angularSpeed;
}

sf::Vector2f CircularMotion::getPositionAtTime(float t) const
{
    // Calculate the position of the shape at time t
    float x = center.x + radius * std::cos(angle + angularSpeed * t);
    float y = center.y + radius * std::sin(angle + angularSpeed * t);
    return {x, y};
}

ei::Vec2 CircularMotion::getPositionAtTimeEi(float t) const
{
    // Calculate the position of the shape at time t
    float x = center.x + radius * std::cos(angle + angularSpeed * t);
    float y = center.y + radius * std::sin(angle + angularSpeed * t);
    return {x, y};
}

float CircularMotion::findCollisionTime(float t0, float t1, float tolerance, collision_detection::BoundingVolume& movingObject, const collision_detection::BoundingVolume& staticObject) const
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

void CircularMotion::setRadius(float r)
{
    radius = r;
}

float CircularMotion::getRadius() const
{
    return radius;
}

void CircularMotion::setCenter(sf::Vector2f c)
{
    center = c;
}

sf::Vector2f CircularMotion::getCenter() const
{
    return center;
}

void CircularMotion::setAngle(float a)
{
    angle = a;
}

float CircularMotion::getAngle() const
{
    return angle;
}

void CircularMotion::setAngularSpeed(float s)
{
    angularSpeed = s;
}

float CircularMotion::getAngularSpeed() const
{
    return angularSpeed;
}

void CircularMotion::setAngularSpeedIncrement(float i)
{
    angularSpeedIncrement = i;
}

float CircularMotion::getAngularSpeedIncrement() const
{
    return angularSpeedIncrement;
}

void CircularMotion::toggleDirection()
{
    isClockwise = !isClockwise;
}
void CircularMotion::setClockwise()
{
    isClockwise = true;
}
void CircularMotion::setCounterClockwise()
{
    isClockwise = false;
}
bool CircularMotion::IsClockwise()
{
    return isClockwise;
}
bool CircularMotion::IsCounterClockwise()
{
    return !isClockwise;
}
