﻿#include <iostream>

#include <exception>
#include <fstream>
#include <chrono>
#include <ei/2dtypes.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <cxxopts.hpp>

#include "app.h"
#include "TextWindow.h"
#include "VisualAlgorithm.h"
#include "JarvisMarch.h"
#include "QuickHull.h"
#include "DataGenerator.h"

void PrintDuration(std::chrono::high_resolution_clock::time_point start, std::chrono::high_resolution_clock::time_point end) {
    auto duration = end - start;

    auto hours = std::chrono::duration_cast<std::chrono::hours>(duration);
    duration -= hours;
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration);
    duration -= minutes;
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
    duration -= seconds;
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
    duration -= milliseconds;
    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration);
    duration -= microseconds;
    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);

    std::cout << "Time taken: ";
    bool printed = false;
    if (hours.count() > 0) {
        std::cout << hours.count() << " hours ";
        printed = true;
    }
    if (minutes.count() > 0) {
        std::cout << minutes.count() << " minutes ";
        printed = true;
    }
    if (seconds.count() > 0) {
        std::cout << seconds.count() << " seconds ";
        printed = true;
    }
    if (milliseconds.count() > 0) {
        std::cout << milliseconds.count() << " milliseconds ";
        printed = true;
    }
    if (microseconds.count() > 0) {
        std::cout << microseconds.count() << " microseconds ";
        printed = true;
    }
    if (nanoseconds.count() > 0) {
        std::cout << nanoseconds.count() << " nanoseconds";
        printed = true;
    }
    if (!printed) {
        std::cout << "0 nanoseconds";
    }
    std::cout << std::endl;
}

Algorithm ParseAlgorithmOption(const cxxopts::ParseResult& result) {
    int mode = 0;
    try {
        mode = result["algorithm"].as<int>();
    }
    catch (const std::exception &e) {
        std::cerr << "Error parsing algorithm mode: " << e.what() << std::endl;
        return INVALID_ALGORITHM;
    }

    if (mode < 0 || mode > 2) {
        std::cerr << "Invalid algorithm mode: " << mode << std::endl
                  << "Valid modes are: 0: QuickHull, 1: Jarvis March, 2: Divide & Conquer" << std::endl;
        return INVALID_ALGORITHM;
    }
    return static_cast<Algorithm>(mode);
}

std::vector<ei::Vec2> LoadDataFromFile(const std::string& filename) {
    std::vector<ei::Vec2> points;

    std::cout << "Loading data file..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream data_file(filename);
    if (!data_file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return points;
    }

    std::string line;
    if (!std::getline(data_file, line)) {
        std::cerr << "Error reading from file: " << filename << std::endl;
        return points;
    }

    int num_points = 0;
    try {
        num_points = std::stoi(line);
    }
    catch (const std::exception &e) {
        std::cerr << "Error parsing number of points: " << e.what() << std::endl;
        return points;
    }

    for (int i = 0; i < num_points; i++) {
        if (!std::getline(data_file, line)) {
            std::cerr << "Error reading point data from file: " << filename << std::endl;
            points.clear();
            return points;
        }
        std::istringstream iss(line);
        float x, y;
        if (!(iss >> x >> y)) {
            std::cerr << "Error parsing point data on line " << i + 2 << std::endl;
            points.clear();
            return points;
        }
        points.emplace_back(x, y);
    }
    data_file.close();

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Loaded " << num_points << " points from file: " << filename << std::endl;
    PrintDuration(start, end);

    return points;
}

std::vector<ei::Vec2> GenerateData(int test_case, int num_points) {
    std::cout << "Generating data for test case " << generatorToString(static_cast<Generator>(test_case))
        << " with " << num_points << " points..." << std::endl;
    const auto start = std::chrono::high_resolution_clock::now();
    std::vector<ei::Vec2> points = DataGenerator::GeneratePoints(static_cast<Generator>(test_case), num_points);
    const auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Generated " << points.size() << " points." << std::endl;
    PrintDuration(start, end);

    return points;
}

int main(int argc, char *argv[]) {
    std::cout << "Program started." << std::endl;

    if (argc == 1) {
        // Just run the GUI if there's no arguments
        return gui_main(QUICK_HULL);
    }

    // Configure CLI options
    cxxopts::Options options("convex-hull", "Convex Hull Algorithms Visualization and Performance Test by Group (Nikolaus, Markus, Marius)");

    options.add_options()
        ("h,help", "Print help")
        ("g,gui", "Run with visualization using pre-loaded data (limited to less than 50 points)",
         cxxopts::value<bool>()->default_value("false"))
        ("d,data_file", "Path to a file containing points to load", cxxopts::value<std::string>())
        ("a,algorithm", "Algorithm to use as integer. 0: QuickHull, 1: Jarvis March, 2: Divide & Conquer",
         cxxopts::value<int>()->default_value("0"))
        ("t,test", "Perform the provided test case with generated data. 0: Random Points, 1: Straight Line, 2: Circle, 3: Square, 4: Large Dataset",
         cxxopts::value<int>()->default_value("-1"))
        ("n,number", "Number of points to generate for test case", cxxopts::value<int>()->default_value("100"));

    options.parse_positional("data_file");
    options.positional_help("data_file");

    // Parse CLI
    cxxopts::ParseResult result;
    try {
        result = options.parse(argc, argv);
    }
    catch (const cxxopts::exceptions::parsing &e) {
        std::cerr << "Error parsing options: " << e.what() << std::endl;
        std::cerr << options.help() << std::endl;
        return EXIT_FAILURE;
    }

    // Print help
    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        return EXIT_SUCCESS;
    }

    // Parse algorithm option
    Algorithm algorithm = ParseAlgorithmOption(result);
    if (algorithm == INVALID_ALGORITHM) {
        std::cerr << "Invalid algorithm mode specified." << std::endl;
        return EXIT_FAILURE;
    }

    // Check mutual exclusivity of data_file and test options
    bool data_file_provided = result.count("data_file") > 0;
    int test_case = result["test"].as<int>();
    bool test_case_provided = test_case != -1;

    if (data_file_provided && test_case_provided) {
        std::cerr << "Error: Options 'data_file' (-d) and 'test' (-t) are mutually exclusive. Please specify only one." << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<ei::Vec2> points;

    if (data_file_provided) {
        points = LoadDataFromFile(result["data_file"].as<std::string>());
        if (points.empty()) {
            return EXIT_FAILURE; // Error message already printed in LoadDataFromFile
        }
    } else if (test_case_provided) {
        int num_points = result["number"].as<int>();
        if (num_points <= 0) {
            std::cerr << "Number of points must be positive." << std::endl;
            return EXIT_FAILURE;
        }

        if (test_case < 0 || test_case > 4) {
            std::cerr << "Invalid test case specified: " << test_case << ". Valid test cases are 0 to 4." << std::endl;
            return EXIT_FAILURE;
        }
        points = GenerateData(test_case, num_points);
    } else {
        // Neither data_file nor test case is provided
        if (result["gui"].as<bool>()) {
            // Run GUI with default or empty data and specified algorithm
            return gui_main(algorithm);
        }
        std::cerr << "Error: You must specify either a data file (-d), a test case (-t), or run the GUI (-g)." << std::endl;
        std::cerr << options.help() << std::endl;
        return EXIT_FAILURE;
    }

    // Run program
    if (result["gui"].as<bool>()) {
        if (points.size() > VISUALIZATION_POINTS_LIMIT) {
            std::cerr << "Too many points for visualization (Max: " << VISUALIZATION_POINTS_LIMIT
                      << "). Please use console mode." << std::endl;
            return EXIT_FAILURE;
        }
        return gui_main(algorithm, &points);
    }
    auto exit_code =  console_main(algorithm, points);
    points.clear();
    return exit_code;
}

int console_main(Algorithm algorithm, std::vector<ei::Vec2>& loadedPoints)
{
    std::cout << "Running " << algorithmToString(algorithm) << " in console mode..." << std::endl;
    std::vector<ei::Vec2> hull;
    auto start = std::chrono::high_resolution_clock::now();
    switch (algorithm)
    {
        case QUICK_HULL:
            hull = quick_hull_performance(loadedPoints);
            break;
        case JARVIS_MARCH:
            hull = jarvis_march_performance(loadedPoints);
            break;
        case DIVIDE_AND_CONQUER:
            // divideAndConquerAlgorithm(loadedPoints);
            break;
        default:
            std::cerr << "Invalid algorithm mode specified." << std::endl;
            return EXIT_FAILURE;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Convex Hull Points: " << hull.size() << std::endl;
    PrintDuration(start, end);
    return EXIT_SUCCESS;
}

int gui_main(Algorithm algorithm, std::vector<ei::Vec2>* loadedPoints)
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT), "APRG - Convex Hull");
    // Center position (for resetting shape positions)
    sf::Vector2f windowCenter = {WINDOW_DEFAULT_WIDTH / 2.f, WINDOW_DEFAULT_HEIGHT / 2.f};

    // Load a font for text
    sf::Font font;
    // Font file path is relative to binary output. CMakelists.txt copies the font file to the binary directory.
    if (!font.loadFromFile("../resources/FiraCodeNerdFontMono-Retina.ttf"))
    {
        std::cerr << "Error loading font\n";
        return EXIT_FAILURE;
    }

    sf::VertexArray drawableDots;
    std::vector<ei::Vec2> points;

    // Create algorithm holder for step visualization
    VisualAlgorithm alg_holder;
    switch (algorithm)
    {
        case QUICK_HULL:
            alg_holder = VisualAlgorithm(quick_hull_visualization);
            break;
        case JARVIS_MARCH:
            alg_holder = VisualAlgorithm(jarvis_march_visualization);
            break;
        case DIVIDE_AND_CONQUER:
            // alg_holder = VisualAlgorithm(divideAndConquerAlgorithm);
            break;
    }
    alg_holder.setFont(font);

    // Create TextWindow (for information fields)
    TextWindow textWindow(font);

    // Handle pre-loaded points
    if(loadedPoints)
    {
        points = *loadedPoints;
        for (const auto &point : points)
        {
            auto dot = sf::Vertex(sf::Vector2f(point.x, point.y), sf::Color::White);
            drawableDots.append(dot);
            auto index = textWindow.addTextField(sf::Vector2f(point.x, point.y), 0, 30);
            textWindow.setText(index, "X: " + std::to_string(point.x) + "\nY: " + std::to_string(point.y));
        }
        alg_holder.setInput(points);
    }

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
                textWindow.setText(index, "X: " + formatFloat(mousePos.x, 2) + "\nY: " + formatFloat(mousePos.y, 2));
                std::cout << "Added point at: (" << mousePos.x << "," << mousePos.y << ")" << std::endl;
                alg_holder.setInput(points);
            }

            if (event.type == sf::Event::KeyPressed)
            {
                switch (event.key.code)
                {
                case sf::Keyboard::Enter:
                    alg_holder.runAlgorithm(window);
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

        // Clear the window with a black color
        window.clear(sf::Color::Black);

        window.draw(drawableDots);
        textWindow.draw(window);
        alg_holder.draw(window);

        window.display();
    }
    return EXIT_SUCCESS;
}