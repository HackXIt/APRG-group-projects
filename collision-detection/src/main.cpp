#include <CircularMotion.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "app.h"
#include "ConfigurationWindow.hpp"

const int MAXIMUM_DAMAGE = 50;

int main()
{
    // Create a window with SFML
    sf::RenderWindow window(sf::VideoMode(WINDOW_DEFAULT_HEIGHT, WINDOW_DEFAULT_HEIGHT), "APRG - Dynamic Intersection Tests");

    // Set up the sphere (circle shape in 2D)
    sf::CircleShape wreckingBall(MAXIMUM_DAMAGE); // Radius of 50 pixels
    wreckingBall.setFillColor(sf::Color::Red);

    // Create a circular motion object for the sphere
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
