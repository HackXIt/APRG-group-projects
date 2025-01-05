//
// Created by RINI on 21/12/2024.
//

#include <cxxopts.hpp> // Include the external cxxopts library
#include <iostream>
#include <fstream>
#include <string>
#include "game_of_life.hpp"
#include "Timing.h"

#ifdef GUI
#include "gui.hpp"
#endif

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
            ("p,pretty", "Pretty print the measurement results", cxxopts::value<bool>()->default_value("false"))
            ("csv", "Write time measurements to a CSV file", cxxopts::value<bool>()->default_value("false"))
            ("mode", "Configure execution mode ('seq'=='sequential', 'par'|'omp'=='parallel')", cxxopts::value<std::string>()->default_value("seq"))
            ("threads", "Number of threads to use in parallel mode", cxxopts::value<int>()->default_value("4"))
#ifdef GUI
            ("gui", "Enable graphical user interface (arg==cell size)", cxxopts::value<int>()->default_value("25"))
#endif
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

    GameOfLife *game = nullptr;

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
        bool pretty = result["pretty"].as<bool>();
        bool csv = result["csv"].as<bool>();
        bool parallel = false;
        std::string mode = result["mode"].as<std::string>();
        int threads = result["threads"].as<int>();
#ifdef GUI
        int cell_size = result["gui"].as<int>();
        if(result.count("gui"))
        {
            return runGui(*GameOfLife::fromFile(inputFile), cell_size);
        }
#endif

        Timing* timing = nullptr;

        if (measure) {
            timing = Timing::getInstance();
            timing->startSetup();
        }

        if(mode.rfind("seq",0) == 0)
        {
            game = GameOfLife::fromFile(inputFile, parallel);
        }
        else if(mode.rfind("par",0) == 0 || mode.rfind("omp",0) == 0) // Check if mode starts with 'par' or 'omp'
        {
            parallel = true;
            game = GameOfLife::fromFile(inputFile, parallel, threads);
        }
        else
        {
            std::cerr << "Error: Invalid mode. Use 'seq' for sequential mode or 'par' for parallel mode." << std::endl;
            return 1;
        }

        if (measure) {
            timing->stopSetup();
            timing->startComputation();
        }

        // Run generations
        game->update(generations);

        if (measure) {
            timing->stopComputation();
            timing->startFinalization();
        }

        // Save output
        game->toFile(outputFile);

        if (measure) {
            timing->stopFinalization();
            if(pretty)
            {
                timing->print(true);
                std::cout << "Simulation complete." << std::endl;
            } else
            {
                std::cout << timing->getResults() << std::endl;
            }
            if(csv)
            {
                // Write measurements to CSV file, appending
                std::string csvFilename = parallel ? "ai24m040_openmp_time.csv" : "ai24m040_cpu_time.csv";
                std::ofstream time_file(csvFilename, std::ios::app);
                if (time_file.is_open())
                {
                    time_file << timing->getResults() << std::endl;
                    time_file.close();
                }
            }
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        delete game;
        return 1;
    }
    delete game;
    return 0;
}