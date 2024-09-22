#include <CircularMotion.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>

#include "app.h"
#include "ConfigurationWindow.hpp"
#include "Sphere.h"
#include "Triangle.h"
#include "AABB.h"

// (these constants are just joke variable names since the object is a wrecking ball)
const int DEATH_SPIKES = 20; // i.e. point count
const float MAXIMUM_DAMAGE = 50.0f; // i.e. radius
const float PENETRATION_BONUS = 10.0f; // i.e. spike length

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
    collision_detection::Sphere wreckingBallSphere = collision_detection::Sphere();
    wreckingBallSphere.calculateFromShape(wreckingBall);
    // collision_detection::Triangle wreckingBallTriangle = collision_detection::Triangle();
    // wreckingBallTriangle.calculateFromShape(wreckingBall);
    // collision_detection::AABB wreckingBallAABB = collision_detection::AABB();
    // wreckingBallAABB.calculateFromShape(wreckingBall);

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

    // Create Sphere bounding volumes for the other shapes
    collision_detection::Sphere circleSphere;
    circleSphere.calculateFromShape(circle);

    collision_detection::Sphere rectangleSphere;
    rectangleSphere.calculateFromShape(rectangle);

    collision_detection::Sphere triangleSphere;
    triangleSphere.calculateFromShape(triangle);

    // Create a circular motion object for the wrecking ball
    CircularMotion circularMotion(wreckingBall);

    // Load a font for text
    sf::Font font;
    if (!font.loadFromFile("../resources/FiraCodeNerdFontMono-Retina.ttf"))
    {
        std::cerr << "Error loading font\n";
        return 1;
    }

    // Create ConfigurationWindow object
    ConfigurationWindow configWindow(font, circularMotion);

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
        }

        // Clear the window with a black color
        window.clear(sf::Color::Black);

        // Update the circular motion
        circularMotion.update();

        // Draw the shape
        window.draw(wreckingBall);

        // Draw the collidable objects
        window.draw(circle);
        window.draw(rectangle);
        window.draw(triangle);

        // Update the bounding volume to match the new shape position
        wreckingBallSphere.calculateFromShape(wreckingBall);
        // wreckingBallTriangle.calculateFromShape(wreckingBall);
        // wreckingBallAABB.calculateFromShape(wreckingBall);

        // NOTE The other shapes are static, so no need to update their bounding volumes

        // Perform intersection tests
        if (wreckingBallSphere.intersects(circleSphere))
        {
            // Collision detected with circle
            circle.setFillColor(sf::Color::Green);
        }
        else
        {
            // No collision
            circle.setFillColor(sf::Color::Red);
        }

        if (wreckingBallSphere.intersects(rectangleSphere))
        {
            // Collision detected with rectangle
            rectangle.setFillColor(sf::Color::Green);
        }
        else
        {
            // No collision
            rectangle.setFillColor(sf::Color::Red);
        }

        if (wreckingBallSphere.intersects(triangleSphere))
        {
            // Collision detected with triangle
            triangle.setFillColor(sf::Color::Green);
        }
        else
        {
            // No collision
            triangle.setFillColor(sf::Color::Red);
        }

        // Draw the bounding volume (debugging)
        wreckingBallSphere.draw(window);
        // wreckingBallTriangle.draw(window);
        // wreckingBallAABB.draw(window);
        triangleSphere.draw(window);
        circleSphere.draw(window);
        rectangleSphere.draw(window);

        // Draw the configuration window
        configWindow.draw(window);

        // Display the contents of the window
        window.display();
    }

    return 0;
}
