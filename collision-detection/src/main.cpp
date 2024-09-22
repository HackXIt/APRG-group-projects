#include <CircularMotion.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>

#include "app.h"
#include "ConfigurationWindow.hpp"

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

        // Draw the configuration window
        configWindow.draw(window);

        // Display the contents of the window
        window.display();
    }

    return 0;
}
