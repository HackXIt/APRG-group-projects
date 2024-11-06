//
// Created by RINI on 06/11/2024.
//

#ifndef MAZE_SOLVER_H
#define MAZE_SOLVER_H

#include <vector>
#include <string>

const int ROWS = 41;
const int COLS = 41;
const char WALL = '#';
const char PATH = '.';
const char SOLUTION_PATH = 'o';
struct Node {
    int row, col, g, h;
    bool isCardinalMove; // To track if the move was cardinal
    Node* parent;

    Node(int r, int c, int g_cost, int h_cost, bool cardinal, Node* p = nullptr)
        : row(r), col(c), g(g_cost), h(h_cost), isCardinalMove(cardinal), parent(p) {}

    int f() const { return g + h; }

    bool operator<(const Node& other) const {
        // Prioritize nodes with lower f scores, and strictly prefer cardinal moves if f scores are equal
        return (f() == other.f()) ? !isCardinalMove && other.isCardinalMove : f() > other.f();
    }
};

void solveMaze(std::vector<std::string>& maze);

#endif //MAZE_SOLVER_H
