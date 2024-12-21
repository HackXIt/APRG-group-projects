//
// Created by RINI on 21/12/2024.
//

#include <cxxopts.hpp> // Include the external cxxopts library
#include <iostream>
#include <string>
#include "game_of_life.h"
#include "Timing.h"

int main(int argc, char* argv[]) {
    // Initialize cxxopts
    std::unique_ptr<cxxopts::Options> options(new cxxopts::Options(argv[0], " - Simulate Conway's Game of Life"));
    auto& opts = *options;

    try
    {
        opts.add_options()
            ("l,load", "Filename to read input board", cxxopts::value<std::string>())
            ("s,save", "Filename to save output board", cxxopts::value<std::string>()->default_value("output.gol"))
            ("g,generations", "Number of generations to simulate", cxxopts::value<int>()->default_value("100"))
            ("m,measure", "Print time measurements", cxxopts::value<bool>()->default_value("false"))
            ("h,help", "Print usage");
    } catch (const cxxopts::exceptions::specification& e)
    {
        std::cerr << "Error setting up options: " << e.what() << std::endl;
        return 1;
    }

    cxxopts::ParseResult result;
    try {
        result = opts.parse(argc, argv);
    } catch (const cxxopts::exceptions::exception& e)
    {
        std::cerr << "Error parsing options: " << e.what() << std::endl;
        return 1;
    }

    try {
        // Help requested
        if (result.count("help")) {
            std::cout << opts.help() << std::endl;
            return 0;
        }

        // Validate input file
        if (!result.count("load")) {
            std::cerr << "Error: Input file (--load) is required." << std::endl;
            return 1;
        }

        // Parse arguments
        std::string inputFile = result["load"].as<std::string>();
        std::string outputFile = result["save"].as<std::string>();
        int generations = result["generations"].as<int>();
        bool measure = result["measure"].as<bool>();

        Timing* timing = nullptr;
        if (measure) {
            timing = Timing::getInstance();
            timing->startSetup();
        }

        // Initialize Game of Life
        GameOfLife game = *GameOfLife::fromFile(inputFile);

        if (measure) {
            timing->stopSetup();
            timing->startComputation();
        }

        // Run generations
        game.update(generations);

        if (measure) {
            timing->stopComputation();
            timing->startFinalization();
        }

        // Save output
        game.toFile(outputFile);

        if (measure) {
            timing->stopFinalization();
            std::cout << timing->getResults() << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "Simulation complete." << std::endl;
    return 0;
}