#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include "maze_solver.h"

using namespace std;

const pair<int, int> START_POS = {1, 1};  // Start from the first accessible position
const pair<int, int> END_POS = {ROWS - 2, COLS - 2}; // End just before the last row

// Chebyshev distance for diagonal movement
int heuristic(int row, int col) {
    return max(abs(row - END_POS.first), abs(col - END_POS.second));
}

bool isValid(int row, int col, const vector<string>& maze, vector<vector<bool>>& visited) {
    return row > 0 && row < ROWS - 1 && col > 0 && col < COLS - 1 &&
           maze[row][col] != WALL && !visited[row][col];
}

void printSolution(vector<string>& maze, Node* endNode) {
    Node* current = endNode;
    while (current) {
        if (maze[current->row][current->col] == PATH)
            maze[current->row][current->col] = SOLUTION_PATH;
        current = current->parent;
    }
}

void solveMaze(vector<string>& maze) {
    vector<vector<bool>> visited(ROWS, vector<bool>(COLS, false));
    priority_queue<Node> openList;

    // Start from the first accessible position inside the boundary
    openList.emplace(START_POS.first, START_POS.second, 0, heuristic(START_POS.first, START_POS.second), true);
    visited[START_POS.first][START_POS.second] = true;

    // Movement in 8 directions (4 cardinal + 4 diagonal)
    const int dRow[] = {-1, 1, 0, 0, -1, -1, 1, 1};
    const int dCol[] = {0, 0, -1, 1, -1, 1, -1, 1};
    const bool isCardinal[] = {true, true, true, true, false, false, false, false};

    while (!openList.empty()) {
        Node current = openList.top();
        openList.pop();

        if (current.row == END_POS.first && current.col == END_POS.second) {
            printSolution(maze, &current);
            return;
        }

        // Expand cardinal moves first, then diagonals if necessary
        for (int i = 0; i < 8; ++i) {
            int newRow = current.row + dRow[i];
            int newCol = current.col + dCol[i];
            bool nextMoveIsCardinal = isCardinal[i];

            if (isValid(newRow, newCol, maze, visited)) {
                visited[newRow][newCol] = true;

                // Prefer cardinal moves by setting a higher gCost for diagonal moves
                int gCost = current.g + (nextMoveIsCardinal ? 1 : 2);
                int hCost = heuristic(newRow, newCol);
                openList.emplace(newRow, newCol, gCost, hCost, nextMoveIsCardinal, new Node(current));
            }
        }
    }
}
