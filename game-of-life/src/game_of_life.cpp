//
// Created by RINI on 21/12/2024.
//

#include "game_of_life.h"
#include <fstream>
#include <stdexcept>
#include <iostream>

GameOfLife::GameOfLife(int rows, int columns, const std::vector<std::vector<char>>& seed)
    : rows(rows), columns(columns), grid(seed) {
    if (seed.size() != rows || seed[0].size() != columns) {
        throw std::invalid_argument("Seed dimensions do not match rows and columns.");
    }
}

void GameOfLife::update() {
    std::vector<std::vector<char>> nextGrid = grid;
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < columns; ++col) {
            int livingNeighbors = countLivingNeighbors(row, col);
            if (grid[row][col] == 'x') { // Live cell
                if (livingNeighbors < 2 || livingNeighbors > 3) {
                    nextGrid[row][col] = '.'; // Cell dies
                }
            } else { // Dead cell
                if (livingNeighbors == 3) {
                    nextGrid[row][col] = 'x'; // Cell becomes alive
                }
            }
        }
    }
    grid = nextGrid;
}

const std::vector<std::vector<char>>& GameOfLife::getGrid() const {
    return grid;
}

GameOfLife GameOfLife::fromFile(const std::string& filename) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        throw std::runtime_error("Failed to open file.");
    }

    int rows, columns;
    inputFile >> rows >> columns;
    inputFile.ignore(); // Skip newline

    std::vector<std::vector<char>> seed(rows, std::vector<char>(columns));
    for (int i = 0; i < rows; ++i) {
        std::string line;
        std::getline(inputFile, line);
        for (int j = 0; j < columns; ++j) {
            seed[i][j] = line[j];
        }
    }

    return GameOfLife(rows, columns, seed);
}

void GameOfLife::toFile(const std::string& filename) const {
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        throw std::runtime_error("Failed to open file.");
    }

    outputFile << rows << "," << columns << "\n";
    for (const auto& row : grid) {
        for (char cell : row) {
            outputFile << cell;
        }
        outputFile << "\n";
    }
}

int GameOfLife::countLivingNeighbors(int row, int col) const {
    int count = 0;
    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            if (dr == 0 && dc == 0) continue;
            int neighborRow = (row + dr + rows) % rows; // Wrap-around
            int neighborCol = (col + dc + columns) % columns; // Wrap-around
            if (grid[neighborRow][neighborCol] == 'x') {
                ++count;
            }
        }
    }
    return count;
}