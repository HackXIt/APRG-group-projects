//
// Created by RINI on 21/12/2024.
//

#include "game_of_life.h"
#include <fstream>
#include <stdexcept>
#include <iostream>

GameOfLife::GameOfLife(int rows, int columns, const std::vector<std::vector<char>>& seed)
    : rows(rows), columns(columns), grid(seed) {
    if (seed.size() != rows || (rows > 0 && seed[0].size() != columns)) {
        throw std::invalid_argument("Seed dimensions do not match rows and columns.");
    }
}

GameOfLife::~GameOfLife() = default;

void GameOfLife::update(int generations) {
    for (int i = 0; i < generations; ++i) {
        std::vector<std::vector<char>> nextGrid(rows, std::vector<char>(columns, '.'));
        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < columns; ++col) {
                int livingNeighbors = countLivingNeighbors(row, col);
                if (grid[row][col] == 'x') { // Live cell
                    if (livingNeighbors >= RULE_STAY_ALIVE_MIN && livingNeighbors <= RULE_STAY_ALIVE_MAX) {
                        nextGrid[row][col] = 'x'; // Cell stays alive
                    }
                } else { // Dead cell
                    if (livingNeighbors == RULE_BECOME_ALIVE_NEIGHBORS) {
                        nextGrid[row][col] = 'x'; // Cell becomes alive
                    }
                }
            }
        }
        grid.swap(nextGrid); // Swap to avoid copying
    }
}

const std::vector<std::vector<char>>& GameOfLife::getGrid() const {
    return grid;
}

GameOfLife GameOfLife::fromFile(const std::string& filename) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        throw std::runtime_error("Failed to open file.");
    }

    std::string dimensions;
    std::getline(inputFile, dimensions);
    size_t commaPos = dimensions.find(',');
    if (commaPos == std::string::npos) {
        throw std::runtime_error("Invalid format: Missing dimensions.");
    }

    int rows, columns;
    try {
        columns = std::stoi(dimensions.substr(0, commaPos));
        rows = std::stoi(dimensions.substr(commaPos + 1));
    } catch (const std::exception&) {
        throw std::runtime_error("Invalid dimensions format.");
    }

    std::vector<std::vector<char>> seed;
    seed.reserve(rows);

    std::string line;
    for (int i = 0; i < rows; ++i) {
        std::getline(inputFile, line);

        // Remove carriage return (Windows line endings) or newline (Unix line endings)
        if (!line.empty() && (line.back() == '\r' || line.back() == '\n')) {
            line.pop_back();
        }
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        if (line.size() != static_cast<size_t>(columns)) {
            throw std::runtime_error("[Row " + std::to_string(i) + "] Size of row (" + std::to_string(line.size()) + ") does not match expected columns (" + std::to_string(columns) + ").");
        }
        seed.emplace_back(line.begin(), line.end());
    }

    inputFile.close();
    return GameOfLife(rows, columns, seed);
}

void GameOfLife::toFile(const std::string& filename) const {
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        throw std::runtime_error("Failed to open file.");
    }

    outputFile << columns << "," << rows << std::endl;
    for (const auto& row : grid) {
        outputFile.write(row.data(), static_cast<std::streamsize>(row.size()));
        outputFile << std::endl; // End of row
    }
    outputFile.close();
}

int GameOfLife::countLivingNeighbors(int row, int col) const {
    static const int directions[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        { 0, -1},          { 0, 1},
        { 1, -1}, { 1, 0}, { 1, 1}
    };

    int count = 0;
    for (const auto& dir : directions) {
        int neighborRow = (row + dir[0] + rows) % rows; // Wrap-around
        int neighborCol = (col + dir[1] + columns) % columns; // Wrap-around
        count += (grid[neighborRow][neighborCol] == 'x');
    }
    return count;
}