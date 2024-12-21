//
// Created by RINI on 21/12/2024.
//

#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H

#include <vector>
#include <string>

#define RULE_BECOME_ALIVE_NEIGHBORS 3
#define RULE_STAY_ALIVE_MIN 2
#define RULE_STAY_ALIVE_MAX 3

class GameOfLife {
public:
    GameOfLife(int rows, int columns, const std::vector<std::vector<char>>& seed);
    ~GameOfLife();

    void update(int generations); // Advance to the next generation

    const std::vector<std::vector<char>>& getGrid() const; // Get the current grid

    static GameOfLife fromFile(const std::string& filename); // Initialize game from file

    void toFile(const std::string& filename) const; // Save current grid to file

private:
    int rows;
    int columns;
    std::vector<std::vector<char>> grid;

    int countLivingNeighbors(int row, int col) const; // Count living neighbors of a cell
};

#endif //GAME_OF_LIFE_H
