#include <iostream>

#include <coroutine>
#include <cstdint>
#include <exception>
#include <fstream>
#include <ei/2dtypes.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>

#include "app.h"
#include "TextWindow.h"
#include "VisualAlgorithm.h"
#include "cxxopts.hpp"
// #include "Quickhull.h"



int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        gui_main();
    }
    cxxopts::Options options("convex-hull", "Convex Hull Algorithms Visualization and Performance Test by Group (Nikolaus, Markus, Marius)");

    options.add_options()
        ("h,help", "Print help")
        ("g,gui", "Run with visualization using pre-loaded data (limited to less than 20 points)", cxxopts::value<bool>()->default_value("false"), "FLAG (bool)")
        ("d,data_file", "Path to a file containing points to load\n 1st line == amount of data points\n line 2...n+1 == points with x,y coordinates (space-seperated)", cxxopts::value<std::string>(), "FILEPATH (string)");
    options.parse_positional({"data_file"});

    cxxopts::ParseResult result;
    try
    {
        result = options.parse(argc, argv);
    }
    catch (const cxxopts::exceptions::parsing &e)
    {
        std::cerr << "Error parsing options: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    if (result.count("help"))
    {
        std::cout << options.help() << std::endl;
        return EXIT_SUCCESS;
    }
    auto filename = result["data_file"].as<std::string>();
    std::string line;
    std::ifstream data_file(filename);
    if(!data_file.is_open())
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        return EXIT_FAILURE;
    }
    std::getline(data_file, line);
    int num_points = std::stoi(line);
    std::vector<ei::Vec2> points;
    for(int i = 0; i < num_points; i++)
    {
        std::getline(data_file, line);
        std::istringstream iss(line);
        float x, y;
        iss >> x >> y;
        points.emplace_back(x, y);
    }
    if (result.count("gui"))
    {
        return gui_main(&points);
    }
    return console_main(points);
}

int console_main(std::vector<ei::Vec2>& loadedPoints)
{
    return EXIT_SUCCESS;
}

int gui_main(std::vector<ei::Vec2>* loadedPoints)
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_DEFAULT_HEIGHT, WINDOW_DEFAULT_HEIGHT), "APRG - Convex Hull");
    // Center position (for resetting shape positions)
    sf::Vector2f windowCenter = {WINDOW_DEFAULT_WIDTH / 2.f, WINDOW_DEFAULT_HEIGHT / 2.f};

    // Load a font for text
    sf::Font font;
    if (!font.loadFromFile("../resources/FiraCodeNerdFontMono-Retina.ttf"))
    {
        std::cerr << "Error loading font\n";
        return EXIT_FAILURE;
    }

    // Clock for measuring time
    sf::Clock clock;

    sf::VertexArray drawableDots;
    std::vector<ei::Vec2> points;
    if(loadedPoints)
    {
        points = *loadedPoints;
    }

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
    return EXIT_SUCCESS;
}