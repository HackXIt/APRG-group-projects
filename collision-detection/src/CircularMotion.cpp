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
