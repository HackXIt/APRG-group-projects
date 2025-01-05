//
// Created by RINI on 21/12/2024.
//

#include "game_of_life.hpp"
#include <omp.h>
#include <cstring>
#include <fstream>
#include <stdexcept>
#include <iostream>

// PUBLIC

GameOfLife::GameOfLife(unsigned int rows, unsigned int columns, bool parallel, unsigned int threads)
    : rows(rows),columns(columns)
{
    gridSize = rows * columns;
    this->parallel = parallel;
    this->threads = threads;
    if (parallel) {
        gridStates = std::vector<bool>(gridSize, false);
        gridCounts = std::vector<unsigned char>(gridSize, 0);
    } else {
        grid = new unsigned char[gridSize];
        prevGrid = new unsigned char[gridSize];
        memset(grid, 0, gridSize);
        memset(prevGrid, 0, gridSize);
    }
}

GameOfLife::GameOfLife(unsigned int rows, unsigned int columns, bool parallel, unsigned int threads, const std::vector<std::vector<char>>& seed)
    : GameOfLife(rows, columns, parallel, threads) {
    if(parallel)
    {
        initialize_from_seed_p(seed);
    }
    else
    {
        initialize_from_seed(seed);
    }
}

GameOfLife::~GameOfLife()
{
    if(parallel)
    {
        gridStates.clear();
        gridCounts.clear();
    }
    else
    {
        delete[] grid;
        delete[] prevGrid;
    }
}

void GameOfLife::setCell(unsigned int row, unsigned int col)
{
    const int w = static_cast<int>(columns); // Width
    const int h = static_cast<int>(rows);   // Height
    const unsigned int gridSize = rows * columns;

    // Offsets to calculate the eight neighboring cells (accounting for wrap-around)
    const int cellOffsetLeft = (col == 0) ? w - 1 : -1;
    const int cellOffsetRight = (col == w - 1) ? -(w - 1) : 1;
    const int cellOffsetUp = (row == 0) ? static_cast<int>(gridSize - w) : -w;
    const int cellOffsetDown = (row == h - 1) ? -static_cast<int>(gridSize - w) : w;

    unsigned char *cellPtr = grid + (row * w) + col;

    // Activate cell
    CELL_ACTIVATE(*cellPtr);

    /* NOTE The eight neighboring cells are:
     * AL A AR
     * L  x  R
     * BL B BR
     * A ... Above
     * B ... Below
     * L ... Left
     * R ... Right
     */
    /* NOTE we do not need to increment the alive counter of this cell
     * since starting from the first living cell
     * each subsequent cell will increment the counters of all its neighbors
     */

    // Change the live counter of neighbor cells
    *(cellPtr + cellOffsetLeft) += CELL_COUNTER_INCREMENT; // L
    *(cellPtr + cellOffsetRight) += CELL_COUNTER_INCREMENT; // R
    *(cellPtr + cellOffsetUp) += CELL_COUNTER_INCREMENT; // A
    *(cellPtr + cellOffsetDown) += CELL_COUNTER_INCREMENT; // B
    *(cellPtr + cellOffsetUp + cellOffsetLeft) += CELL_COUNTER_INCREMENT; // AL
    *(cellPtr + cellOffsetUp + cellOffsetRight) += CELL_COUNTER_INCREMENT; // AR
    *(cellPtr + cellOffsetDown + cellOffsetLeft) += CELL_COUNTER_INCREMENT; // BL
    *(cellPtr + cellOffsetDown + cellOffsetRight) += CELL_COUNTER_INCREMENT; // BR
}

void GameOfLife::clearCell(unsigned int row, unsigned int col)
{
    const int w = static_cast<int>(columns); // Width
    const int h = static_cast<int>(rows);   // Height
    const unsigned int gridSize = rows * columns;

    // Offsets to calculate the eight neighboring cells (accounting for wrap-around)
    const int cellOffsetLeft = (col == 0) ? w - 1 : -1;
    const int cellOffsetRight = (col == w - 1) ? -(w - 1) : 1;
    const int cellOffsetUp = (row == 0) ? static_cast<int>(gridSize - w) : -w;
    const int cellOffsetDown = (row == h - 1) ? -static_cast<int>(gridSize - w) : w;

    unsigned char *cellPtr = grid + (row * w) + col;

    // Deactivate cell
    CELL_DEACTIVATE(*cellPtr);

    // Change the live counter of neighbor cells
    *(cellPtr + cellOffsetLeft) -= CELL_COUNTER_INCREMENT; // L
    *(cellPtr + cellOffsetRight) -= CELL_COUNTER_INCREMENT; // R
    *(cellPtr + cellOffsetUp) -= CELL_COUNTER_INCREMENT; // A
    *(cellPtr + cellOffsetDown) -= CELL_COUNTER_INCREMENT; // B
    *(cellPtr + cellOffsetUp + cellOffsetLeft) -= CELL_COUNTER_INCREMENT; // AL
    *(cellPtr + cellOffsetUp + cellOffsetRight) -= CELL_COUNTER_INCREMENT; // AR
    *(cellPtr + cellOffsetDown + cellOffsetLeft) -= CELL_COUNTER_INCREMENT; // BL
    *(cellPtr + cellOffsetDown + cellOffsetRight) -= CELL_COUNTER_INCREMENT; // BR
}

char GameOfLife::cellState(unsigned int row, unsigned int col) const
{
    if(parallel)
    {
        return gridStates[row * columns + col] ? LIVE_CELL : DEAD_CELL;
    }
    return CELL_IS_ALIVE(grid[row * columns + col]) ? LIVE_CELL : DEAD_CELL;
}

std::vector<std::vector<char>> GameOfLife::getGrid() const
{
    std::vector<std::vector<char>> gridVector(rows, std::vector<char>(columns));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            gridVector[i][j] = cellState(i, j);
        }
    }
    return gridVector;
}

void GameOfLife::next()
{
    // Make copy of the current grid (unaltered)
    memcpy(prevGrid, grid, gridSize);

    // Process all cells in this grid
    unsigned char *cellPtr = prevGrid;
    for(unsigned int row = 0; row < rows; ++row)
    {
        unsigned int col = 0;
        do
        {
            // Skip dead cells with no living neighbors
            while(*cellPtr == CELL_DEAD_NO_NEIGHBORS)
            {
                ++cellPtr; // Move to the next cell
                if(++col >= columns) // Check if we reached the end of the row
                {
                    goto RowDone;
                }
            }

            // Cell is active or has neighbors
            const unsigned int count = CELL_GET_COUNT(*cellPtr);
            if(CELL_IS_ALIVE(*cellPtr))
            {
                // Rule: Any cell with fewer than 2 or more than 3 neighbors dies
                if(count < RULE_STAY_ALIVE_MIN || count > RULE_STAY_ALIVE_MAX)
                {
                    clearCell(row, col);
                }
            }
            else
            {
                // Rule: Any dead cell with exactly 3 neighbors becomes alive
                if(count == RULE_BECOME_ALIVE_NEIGHBORS)
                {
                    setCell(row, col);
                }
            }
            ++cellPtr; // Move to the next cell
        } while(++col < columns);
    RowDone:;
    }
}

void GameOfLife::nextP()
{
    std::vector<bool> prevGridStates = gridStates;
    std::vector<unsigned char> prevGridCounts = gridCounts;
    omp_set_num_threads(threads);
    #pragma omp parallel for
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < columns; ++j)
        {
            // Count neighbors from prevGrid
            unsigned int count = 0;
            // A small function to wrap index around (toroidal):
            auto idx = [&](unsigned int r, unsigned int c) {
                // wrap row and col for boundaries:
                r = (r + rows) % rows;
                c = (c + columns) % columns;
                return r * columns + c;
            };
            // Check all 8 neighbors
            for (int dr = -1; dr <= 1; ++dr) {
                for (int dc = -1; dc <= 1; ++dc) {
                    if (dr == 0 && dc == 0) continue; // skip self
                    if (prevGridStates[idx(i + dr, j + dc)]) {
                        count++;
                    }
                }
            }
            // Update cell state
            if (prevGridStates[i * columns + j]) {
                // Rule: Any cell with fewer than 2 or more than 3 neighbors dies
                if (count < RULE_STAY_ALIVE_MIN || count > RULE_STAY_ALIVE_MAX) {
                    gridStates[i * columns + j] = false;
                }
            } else {
                // Rule: Any dead cell with exactly 3 neighbors becomes alive
                if (count == RULE_BECOME_ALIVE_NEIGHBORS) {
                    gridStates[i * columns + j] = true;
                }
            }
        }
    }
}



void GameOfLife::update(int generations) {
    if(parallel)
    {
        for (int i = 0; i < generations; ++i) {
            nextP();
        }
    }
    else
    {
        for (int i = 0; i < generations; ++i) {
            next();
        }
    }
}

GameOfLife* GameOfLife::fromFile(const std::string& filename, bool parallel, unsigned int threads) {
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
    if (rows <= 0 || columns <= 0) {
        throw std::runtime_error("Invalid dimensions.");
    }
    const auto game = new GameOfLife(rows, columns);

    std::string line;
    for (int i = 0; i < rows; ++i) {
        std::getline(inputFile, line);
        if (line.size() != static_cast<size_t>(columns)) {
            throw std::runtime_error("[Row " + std::to_string(i) + "] Size of row (" + std::to_string(line.size()) + ") does not match expected columns (" + std::to_string(columns) + ").");
        }
        for (int j = 0; j < columns; ++j)
        {
            if(line[j] == '\r' || line[j] == '\n') {
                continue;
            }
            if (line[j] == LIVE_CELL) {
                game->setCell(i, j);
            }
            // NOTE Cells are initialized as dead by default
        }
    }

    inputFile.close();
    return game;
}

void GameOfLife::toFile(const std::string& filename) const {
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        throw std::runtime_error("Failed to open file.");
    }

    outputFile << columns << "," << rows << std::endl;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            outputFile << cellState(i, j);
        }
        outputFile << std::endl;
    }
    outputFile.close();
}

// PRIVATE

void GameOfLife::initialize_from_seed(const std::vector<std::vector<char>>& seed)
{
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            if (seed[i][j] == LIVE_CELL) {
                setCell(i, j);
            }
            // NOTE Cells are initialized as dead by default
        }
    }
}

void GameOfLife::initialize_from_seed_p(const std::vector<std::vector<char>>& seed)
{
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            if (seed[i][j] == LIVE_CELL) {
                gridStates[i * columns + j] = true;

            }
            // NOTE Cells are initialized as dead by default
        }
    }
}