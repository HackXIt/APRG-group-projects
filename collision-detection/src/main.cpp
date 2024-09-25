#include <CircularMotion.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>

#include "app.h"
#include "ConfigurationWindow.hpp"
#include "Sphere.h"
#include "Triangle.h"
#include "AABB.h"
#include "InformationWindow.hpp"
#include "Plane.h"
#include "VisualVector.h"

// (these constants are just joke variable names since the object is a wrecking ball)
const int DEATH_SPIKES = 20; // i.e. point count
const float MAXIMUM_DAMAGE = 50.0f; // i.e. radius
const float PENETRATION_BONUS = 10.0f; // i.e. spike length

// Motion switch
// #define CIRCULAR_MOTION
#ifndef CIRCULAR_MOTION
#define LINEAR_MOTION
#endif

int main()
{
    // Create a window with SFML
    sf::RenderWindow window(sf::VideoMode(WINDOW_DEFAULT_HEIGHT, WINDOW_DEFAULT_HEIGHT), "APRG - Dynamic Intersection Tests");
    // Center position (for resetting shape positions)
    sf::Vector2f windowCenter = {WINDOW_DEFAULT_WIDTH / 2.f, WINDOW_DEFAULT_HEIGHT / 2.f};

    // Set up the moving shape (2D - circle shape with spikes)
    const int pointCount = DEATH_SPIKES; // NOTE Must be an even number
    const float radius = MAXIMUM_DAMAGE;
    const float spikeLength = PENETRATION_BONUS;
    sf::ConvexShape wreckingBall(pointCount);

    // Calculate the points
    for (int i = 0; i < pointCount; ++i) {
        float angle = i * (2 * M_PI / pointCount) - M_PI / 2; // Start from top
        float currentRadius = (i % 2 == 0) ? radius : (radius + spikeLength);
        float x = currentRadius * std::cos(angle);
        float y = currentRadius * std::sin(angle);
        wreckingBall.setPoint(i, sf::Vector2f(x, y));
    }

    wreckingBall.setFillColor(sf::Color::Blue);
    wreckingBall.setOutlineColor(sf::Color::White);
    wreckingBall.setOutlineThickness(2.0f);
    wreckingBall.setPosition(windowCenter);

    // Create a bounding volume object for the wrecking ball
    collision_detection::Sphere wreckingBallSphere = collision_detection::Sphere(&wreckingBall);
    // collision_detection::Triangle wreckingBallTriangle = collision_detection::Triangle(wreckingBall);
    // collision_detection::AABB wreckingBallAABB = collision_detection::AABB(wreckingBall);

    // Create multiple objects to collide with (Circle, Rectangle, Triangle - Walls of different shape)
    sf::CircleShape circle(50);
    circle.setFillColor(sf::Color::Red);
    circle.setOutlineColor(sf::Color::White);
    circle.setOutlineThickness(2.0f);
    circle.setOrigin(50, 50);
    circle.setPosition(windowCenter + sf::Vector2f(200, 0));

    sf::RectangleShape rectangle(sf::Vector2f(100, 100));
    rectangle.setFillColor(sf::Color::Red);
    rectangle.setOutlineColor(sf::Color::White);
    rectangle.setOutlineThickness(2.0f);
    rectangle.setOrigin(50, 50);
    rectangle.setPosition(windowCenter + sf::Vector2f(0, 200));

    sf::ConvexShape triangle(3);
    triangle.setPoint(0, sf::Vector2f(0, 0));
    triangle.setPoint(1, sf::Vector2f(100, 0));
    triangle.setPoint(2, sf::Vector2f(50, 100));
    triangle.setFillColor(sf::Color::Red);
    triangle.setOutlineColor(sf::Color::White);
    triangle.setOutlineThickness(2.0f);
    triangle.setOrigin(50, 50);
    triangle.setPosition(windowCenter + sf::Vector2f(-200, 0));

    sf::Vertex line[2];
    line[0].position = sf::Vector2f(640, 252);
    line[0].color = sf::Color::Yellow;
    line[1].position = sf::Vector2f(740, 252);
    line[1].color = sf::Color::Yellow;

    // Create Sphere bounding volumes for the other shapes
    collision_detection::Sphere circleSphere(&circle);
    collision_detection::AABB rectangleSphere(&rectangle);
    collision_detection::Triangle triangleAABB(&triangle);
    collision_detection::Plane linePlane(line);

    // Incremental update
    bool update = false;

    // Load a font for text
    sf::Font font;
    if (!font.loadFromFile("../resources/FiraCodeNerdFontMono-Retina.ttf"))
    {
        std::cerr << "Error loading font\n";
        return 1;
    }

    // Create a motion object for the wrecking ball and a ConfigurationWindow for the motion
#ifdef CIRCULAR_MOTION
    CircularMotion circularMotion(wreckingBall);
    ConfigurationWindow configWindow(font, &circularMotion, nullptr);
#endif
#ifdef LINEAR_MOTION
    LinearMotion linearMotion(wreckingBall);
    ConfigurationWindow configWindow(font, nullptr, &linearMotion);
#endif
    // Create ConfigurationWindow object


    // Create information windows for live data on objects
    InformationWindow infoWindow(font);
    infoWindow.addTextField(&wreckingBall, &wreckingBallSphere, 0, -150);
    infoWindow.addTextField(&circle, &circleSphere);
    infoWindow.addTextField(&rectangle, &rectangleSphere);
    infoWindow.addTextField(&triangle, &triangleAABB);
    infoWindow.addTextField(line[0].position, &linePlane);

    // Clock for measuring time
    sf::Clock clock;

    // Main loop
    while (window.isOpen())
    {
        // Handle events
        sf::Event event{};
        while (window.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
                window.close();

            // Pass events to configuration window
            configWindow.handleEvent(event, window);
#ifdef LINEAR_MOTION
            if (event.type == sf::Event::MouseButtonPressed)
            {
                // Get mouse position
                sf::Vector2i mousePosWindow = sf::Mouse::getPosition(window);
                sf::Vector2f mousePos = window.mapPixelToCoords(mousePosWindow);
                linearMotion.setTarget(mousePos);
            }
#endif

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
            {
                update = true;
            }        //
        // ei::Vec2 current = ei::Vec2(wreckingBall.getPosition().x, wreckingBall.getPosition().y);
        // ei::Vec2 target = ei::Vec2(current + ei::Vec2{15, 15});
        // VisualVector v0(current, target);
        // v0.draw(window);
        }

        // Clear the window with a black color
        window.clear(sf::Color::Black);



        float deltaTime = clock.restart().asSeconds(); // Use SFML clock to get deltaTime
        float t0 = 0.0f;
        float t1 = deltaTime;

        // Update the circular motion
#ifdef CIRCULAR_MOTION
        circularMotion.update();
#endif
#ifdef LINEAR_MOTION
        // Get the velocity from LinearMotion
        // ei::Vec2 velocity = linearMotion.getVelocityVec2();
        // ei::Vec2 d = velocity; // Since deltaTime is t1 - t0 and t1 - t0 = deltaTime
        ei::Vec2 d{15, 15};
        if(update)
        {
            linearMotion.update();
            // Update the bounding volume to match the new shape position
            wreckingBallSphere.calculateFromShape();
            // wreckingBallTriangle.calculateFromShape(wreckingBall);
            // wreckingBallAABB.calculateFromShape(wreckingBall);

            // NOTE The other shapes are static, but we update them anyhow
            circleSphere.calculateFromShape();
            rectangleSphere.calculateFromShape();
            triangleAABB.calculateFromShape();
            linePlane.calculateFromShape();
            // Draw the bounding volume (debugging)
            wreckingBallSphere.draw(window);
            // wreckingBallTriangle.draw(window);
            // wreckingBallAABB.draw(window);
            triangleAABB.draw(window);
            circleSphere.draw(window);
            rectangleSphere.draw(window);
            linePlane.draw(window);
        }
#endif

        bool noIntersection = true;

        // Perform intersection tests
        float collisionTime;

        // For each static object, perform the interval halving collision test
        if (wreckingBallSphere.testMovingSphereSphereVisualContinous(d, t0, t1, circleSphere, collisionTime, window))
        {
            // Collision detected with circle at time collisionTime
            circleSphere.isColliding = true;
            noIntersection = false;

            // Optional: Handle collision response here, e.g., stop motion at collision point
            sf::Vector2f collisionPosition = linearMotion.getPositionAtTime(collisionTime);
            wreckingBall.setPosition(collisionPosition);
            wreckingBallSphere.calculateFromShape();
        }
        else
        {
            circleSphere.isColliding = false;
        }
        // if (wreckingBallSphere.intersects(circleSphere))
        // {
        //     // Collision detected with circle
        //     circleSphere.isColliding = true;
        //     noIntersection = false;
        // }
        // else
        // {
        //     // No collision
        //     circleSphere.isColliding = false;
        // }

        if (wreckingBallSphere.intersects(rectangleSphere))
        {
            // Collision detected with rectangle
            rectangleSphere.isColliding = true;
            noIntersection = false;
        }
        else
        {
            // No collision
            rectangleSphere.isColliding = false;
        }

        if (wreckingBallSphere.intersects(triangleAABB))
        {
            // Collision detected with triangle
            triangleAABB.isColliding = true;
            noIntersection = false;
        }
        else
        {
            // No collision
            triangleAABB.isColliding = false;
        }

        if (wreckingBallSphere.intersects(linePlane))
        {
            // Collision detected with plane
            linePlane.isColliding = true;
            noIntersection = false;
        }
        else
        {
            // No collision
            linePlane.isColliding = false;
        }

        if (!noIntersection)
        {
#ifdef LINEAR_MOTION
            // Stop the motion at the collision point
            // For LinearMotion
            sf::Vector2f collisionPosition = linearMotion.getPositionAtTime(collisionTime);
            // linearMotion.setPosition(collisionPosition);
            // wreckingBall.setPosition(collisionPosition);
            wreckingBallSphere.calculateFromShape();
#endif
            wreckingBallSphere.isColliding = true;
        }
        else
        {
#ifdef LINEAR_MOTION
#endif
            wreckingBallSphere.isColliding = false;
        }


        // Shapes are drawn by their bounding volumes

        // Draw the shape
        // window.draw(wreckingBall);

        // Draw the collidable objects
        // window.draw(circle);
        // window.draw(rectangle);
        // window.draw(triangle);

        if(!update)
        {
            // Draw the bounding volume (debugging)
            wreckingBallSphere.draw(window);
            // wreckingBallTriangle.draw(window);
            // wreckingBallAABB.draw(window);
            triangleAABB.draw(window);
            circleSphere.draw(window);
            rectangleSphere.draw(window);
            linePlane.draw(window);
        }

        // Draw the configuration window
        configWindow.draw(window);

        // Draw the information window
        infoWindow.draw(window);

        update = false;

        // Display the contents of the window
        window.display();
    }

    return 0;
}
