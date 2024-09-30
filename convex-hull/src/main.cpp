#include <iostream>

#include <coroutine>
#include <cstdint>
#include <exception>
#include <ei/2dtypes.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>

#include "app.h"
#include "TextWindow.h"
#include "VisualAlgorithm.h"
#include "Quickhull.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_DEFAULT_HEIGHT, WINDOW_DEFAULT_HEIGHT), "APRG - Convex Hull");
    // Center position (for resetting shape positions)
    sf::Vector2f windowCenter = {WINDOW_DEFAULT_WIDTH / 2.f, WINDOW_DEFAULT_HEIGHT / 2.f};

    // Load a font for text
    sf::Font font;
    if (!font.loadFromFile("../resources/FiraCodeNerdFontMono-Retina.ttf"))
    {
        std::cerr << "Error loading font\n";
        return 1;
    }

    // Clock for measuring time
    sf::Clock clock;

    sf::VertexArray drawableDots;
    std::vector<ei::Vec2> points;

    // Create algorithm holder for step visualization
    //VisualAlgorithm alg_holder(exampleAlgorithm);
    VisualAlgorithm alg_holder(quickHullAlgorithm);
    alg_holder.setFont(font);

    // Create TextWindow (for information fields)
    TextWindow textWindow(font);

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

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (alg_holder.IsStarted())
                {
                    std::cout << "Cannot add points after algorithm calculation started. Please complete calculation." << std::endl;
                    continue;
                }
                // Get mouse position
                sf::Vector2i mousePosWindow = sf::Mouse::getPosition(window);
                sf::Vector2f mousePos = window.mapPixelToCoords(mousePosWindow);
                auto dot = sf::Vertex(mousePos, sf::Color::White);
                drawableDots.append(dot);
                points.emplace_back(mousePos.x, mousePos.y);
                auto index = textWindow.addTextField(mousePos, 0, 30);
                textWindow.setText(index, "X: " + std::to_string(mousePos.x) + "\nY: " + std::to_string(mousePos.y));
                std::cout << "Added point at: (" << mousePos.x << "," << mousePos.y << ")" << std::endl;
                alg_holder.setInput(points);
            }

            if (event.type == sf::Event::KeyPressed)
            {
                switch (event.key.code)
                {
                case sf::Keyboard::Enter:
                    alg_holder.runAlgorithm();
                    break;
                case sf::Keyboard::Space:
                    alg_holder.visualStep();
                    break;
                case sf::Keyboard::R:
                    std::cout << "Resetting algorithm" << std::endl;
                    alg_holder.reset();
                    break;
                default:
                    std::cout << "Key: " << event.key.code << " not used/supported" << std::endl;
                    break;
                }
            }
        }

        window.draw(drawableDots);
        textWindow.draw(window);
        alg_holder.draw(window);

        window.display();
    }
}
