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
    line[0].position = sf::Vector2f(640, 352);
    line[0].color = sf::Color::Yellow;
    line[1].position = sf::Vector2f(740, 352);
    line[1].color = sf::Color::Yellow;

    // Create Sphere bounding volumes for the other shapes
    collision_detection::Sphere circleSphere(&circle);
    collision_detection::AABB rectangleSphere(&rectangle);
    collision_detection::Triangle triangleAABB(&triangle);
    collision_detection::Plane linePlane(line);

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
        }

        // Clear the window with a black color
        window.clear(sf::Color::Black);

        // Update the circular motion
#ifdef CIRCULAR_MOTION
        circularMotion.update();
#endif
#ifdef LINEAR_MOTION
        linearMotion.update();
#endif

        // Update the bounding volume to match the new shape position
        wreckingBallSphere.calculateFromShape();
        // wreckingBallTriangle.calculateFromShape(wreckingBall);
        // wreckingBallAABB.calculateFromShape(wreckingBall);

        // NOTE The other shapes are static, but we update them anyhow
        circleSphere.calculateFromShape();
        rectangleSphere.calculateFromShape();
        triangleAABB.calculateFromShape();
        linePlane.calculateFromShape();

        bool noIntersection = true;

        // Perform intersection tests
        if (wreckingBallSphere.intersects(circleSphere))
        {
            // Collision detected with circle
            circleSphere.isColliding = true;
            noIntersection = false;
        }
        else
        {
            // No collision
            circleSphere.isColliding = false;
        }

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

        if (noIntersection)
        {
            wreckingBallSphere.isColliding = false;
        }
        else
        {
            wreckingBallSphere.isColliding = true;
        }

        // Shapes are drawn by their bounding volumes

        // Draw the shape
        // window.draw(wreckingBall);

        // Draw the collidable objects
        // window.draw(circle);
        // window.draw(rectangle);
        // window.draw(triangle);

        // Draw the bounding volume (debugging)
        wreckingBallSphere.draw(window);
        // wreckingBallTriangle.draw(window);
        // wreckingBallAABB.draw(window);
        triangleAABB.draw(window);
        circleSphere.draw(window);
        rectangleSphere.draw(window);
        linePlane.draw(window);

        // Draw the configuration window
        configWindow.draw(window);

        // Draw the information window
        infoWindow.draw(window);

        // Display the contents of the window
        window.display();
    }

    return 0;
}
