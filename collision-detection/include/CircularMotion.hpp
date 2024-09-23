//
// Created by RINI on 20/09/2024.
//

#ifndef CIRCULARMOTION_H
#define CIRCULARMOTION_H
#include "SFML/Graphics/Shape.hpp"
#include "SFML/System/Vector2.hpp"
#include "ei/2dtypes.hpp"
#include "app.h"
#include "BoundingVolume.h"

constexpr float CIRCULARMOTION_RADIUS_DEFAULT = 150.0f; ///< Default radius of the circular path.
const sf::Vector2f CIRCULARMOTION_CENTER_DEFAULT = {WINDOW_DEFAULT_WIDTH / 2.f, WINDOW_DEFAULT_HEIGHT / 2.f}; ///< Default center of the circular path.
constexpr float CIRCULARMOTION_ANGLE_DEFAULT = 0.0f; ///< Default starting angle in radians.
constexpr float CIRCULARMOTION_ANGULARSPEED_DEFAULT = 0.005f; ///< Default speed of rotation (radians per frame).
constexpr float CIRCULARMOTION_ANGULARSPEED_INCREMENT_DEFAULT = 0.001f; ///< Default increment for angular speed.


/**
 * @class CircularMotion
 * @brief A class that handles circular motion of a given SFML shape.
 *
 * This class allows you to set up and update the position of an SFML shape to move it in a circular path.
 * It provides properties to configure the radius, center point, angle, and angular speed of the motion.
 */
class CircularMotion {
  public:
 /**
  * @brief Constructor for CircularMotion.
  *
  * @param shape Reference to the SFML shape to move in circular motion.
  * @param radius Radius of the circular path.
  * @param center Center point of the circular path.
  * @param angle Starting angle of the motion (in radians).
  * @param angularSpeed Speed at which the shape rotates (in radians per frame).
  * @param angularSpeedIncrement Increment for the angular speed.
  * @param isClockwise Flag to indicate whether the motion is clockwise or counter-clockwise.
  */
 explicit CircularMotion(sf::Shape& shape,
  float radius = CIRCULARMOTION_RADIUS_DEFAULT,
  sf::Vector2f center = CIRCULARMOTION_CENTER_DEFAULT,
  float angle = CIRCULARMOTION_ANGLE_DEFAULT,
  float angularSpeed = CIRCULARMOTION_ANGULARSPEED_DEFAULT,
  float angularSpeedIncrement = CIRCULARMOTION_ANGULARSPEED_INCREMENT_DEFAULT,
  bool isClockwise = true);

 /**
  * @brief Updates the position of the shape to move it along the circular path.
  */
 void update();

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

 // Getters and setters for the properties
 void setRadius(float r);
 float getRadius() const;

 void setCenter(sf::Vector2f c);
 sf::Vector2f getCenter() const;

 void setAngle(float a);
 float getAngle() const;

 void setAngularSpeed(float s);
 float getAngularSpeed() const;

 void setAngularSpeedIncrement(float i);
 float getAngularSpeedIncrement() const;

 void toggleDirection();
 void setClockwise();
 void setCounterClockwise();
 bool IsClockwise();
 bool IsCounterClockwise();

private:
 sf::Shape& shapeToRotate; ///< Reference to the shape to move.
 float radius;             ///< Radius of the circular path.
 sf::Vector2f center;      ///< Center point of the circular path.
 float angle;              ///< Current angle of the motion.
 float angularSpeed;       ///< Speed at which the shape rotates.
 float angularSpeedIncrement; ///< Increment for the angular speed.
 bool isClockwise;         ///< Flag to indicate the direction of rotation.
};



#endif //CIRCULARMOTION_H
