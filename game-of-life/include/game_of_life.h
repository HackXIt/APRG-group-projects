//
// Created by RINI on 21/12/2024.
// Optimizations inspired by: https://www.youtube.com/watch?v=ndAfWKmKF34
//

#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H

#include <string>
#include <vector>

#define RULE_BECOME_ALIVE_NEIGHBORS 3
#define RULE_STAY_ALIVE_MIN 2
#define RULE_STAY_ALIVE_MAX 3
#define DEAD_CELL '.'
#define LIVE_CELL 'x'
// NOTE Incrementing alive counter by 2, since the 0th bit is used to store the cell state
// NOTE Adding 0x02 (0b00000010) to the counter, will only affect bits 1-4
#define CELL_COUNTER_INCREMENT 0x02
#define CELL_DEAD_NO_NEIGHBORS 0x00
#define CELL_GET_COUNT(x) ((x >> 1) & 0x0F)
#define CELL_IS_ALIVE(x) (x & 0x01)
#define CELL_ACTIVATE(x) (x |= 0x01)
#define CELL_DEACTIVATE(x) (x &= 0xFE)

class GameOfLife {
public:
    GameOfLife(unsigned int rows, unsigned int columns);
    GameOfLife(unsigned int rows, unsigned int columns, const std::vector<std::vector<char>>& seed);
    ~GameOfLife();

    void next(); // Advance to the next generation
    void update(int generations); // Advance X generations

    void setCell(unsigned int row, unsigned int col); // Set cell state
    void clearCell(unsigned int row, unsigned int col); // Clear cell state
    char cellState(unsigned int row, unsigned int col) const; // Get cell state

    std::vector<std::vector<char>> getGrid() const; // Get current grid as 2D character vector

    static GameOfLife* fromFile(const std::string& filename); // Initialize game from file
    void toFile(const std::string& filename) const; // Save current grid to file

    inline unsigned int getRows() const { return rows; }
    inline unsigned int getColumns() const { return columns; }

private:
    unsigned int rows;
    unsigned int columns;
    unsigned int gridSize;
    // NOTE Bit 0: 0=Dead, 1=Alive; Bits 1-4: Number of living neighbors; Bits 5-7: Unused
    unsigned char* grid; // 1D array to store the grid in unsigned 8-bit
    unsigned char* prevGrid; // 1D array to store the previous grid in unsigned 8-bit
};

#endif //GAME_OF_LIFE_H
