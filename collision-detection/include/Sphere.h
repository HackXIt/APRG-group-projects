//
// Created by RINI on 22/09/2024.
//

#ifndef SPHERE_H
#define SPHERE_H
#include "AABB.h"
#include "BoundingVolume.h"
#include "ei/2dtypes.hpp"

namespace collision_detection {

    class Sphere : public BoundingVolume {
    public:
        ei::Vec2 center;
        float radius;
        ei::Disc2D disc;
        sf::CircleShape debugShape;

        Sphere() : BoundingVolume(nullptr), center(0.0f, 0.0f), radius(0.0f), disc(center, radius)
        {
            debugShape = sf::CircleShape(radius);
            debugShape.setOrigin(radius, radius);
            debugShape.setPosition(center.x, center.y);
            debugShape.setFillColor(sf::Color::Transparent);
            debugShape.setOutlineColor(sf::Color::Yellow);
            debugShape.setOutlineThickness(1.0f);
            debugShape.setFillColor(sf::Color::Transparent);
        }

        Sphere(sf::Shape* shape) : Sphere()
        {
            boundedShape = shape;
        }

        void calculateFromThis();
        void calculateFromShape() override;
        bool intersects(const BoundingVolume& other) const override;
        void draw(sf::RenderWindow& window) const override;

        void drawBounded(sf::RenderWindow& window) const;
        void drawDebug(sf::RenderWindow& window) const;

        std::string toString() const override;

        bool testMovingSphereSphereVisualContinous(const ei::Vec2& d, float t0, float t1, const Sphere& s1, float& collisionTime, sf::RenderWindow& window);

        bool testMovingSphereSphereVisual(
            const ei::Vec2& d,
            float t0,
            float t1,
            const Sphere& s1,
            float& collisionTime,
            sf::RenderWindow& window);
    };

    bool testMovingSphereSphere(
        const Sphere& s0,
        const ei::Vec2& d,
        float t0,
        float t1,
        const Sphere& s1,
        float& collisionTime);
    bool TestMovingSphereAABB(
        const Sphere& s0,
        const ei::Vec2& d,
        float t0, float t1,
        const AABB& s1,
        float& collisionTime);
} // collision_detection

#endif //SPHERE_H
