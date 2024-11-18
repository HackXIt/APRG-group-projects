//
// Created by RINI on 10/11/2024.
//
#include <vector>
#include <string>
#include <iostream>
#include <queue>
#include <algorithm>

using namespace std;

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

// Define the start and end positions for the maze
const pair<int, int> START_POS = {1, 1};  // Start from the first accessible position
const pair<int, int> END_POS = {ROWS - 2, COLS - 2}; // End just before the last row

// Chebyshev distance for diagonal movement
// https://en.wikipedia.org/wiki/Chebyshev_distance
// This heuristic calculates the minimum moves required to reach the target, allowing for diagonal movement.
// It uses the Chebyshev distance formula, which is max(|dx|, |dy|) for grid-based pathfinding with diagonals.
int heuristic(int row, int col) {
    return max(abs(row - END_POS.first), abs(col - END_POS.second));
}

// Function to check if a cell (row, col) is valid for exploration
// Conditions:
// - The cell is within the maze boundaries (excluding outer walls).
// - The cell is not a wall ('#').
// - The cell has not been visited yet
bool isValid(int row, int col, const vector<string>& maze, vector<vector<bool>>& visited) {
    return row > 0 && row < ROWS - 1 && col > 0 && col < COLS - 1 &&
           maze[row][col] != WALL && !visited[row][col];
}

// Function to trace back and mark the solution path in the maze
// Given the end node, it traces back through the parent pointers to mark the path from start to end.
void printSolution(vector<string>& maze, Node* endNode) {
    Node* current = endNode;
    while (current) {
        // Mark only cells that are part of the navigable path with 'o' (SOLUTION_PATH)
        if (maze[current->row][current->col] == PATH)
            maze[current->row][current->col] = SOLUTION_PATH;
        current = current->parent; // Move to the parent node to continue tracing the path
    }
}

// Main function to solve the maze using the A* algorithm
void solveMaze(vector<string>& maze) {
    vector<vector<bool>> visited(ROWS, vector<bool>(COLS, false)); // 2D vector to keep track of visited cells
    // Priority queue (min-heap) for open list in A*, sorted by f() = g + h
    priority_queue<Node> openList;

    // Start from the first accessible position inside the boundary
    openList.emplace(START_POS.first, START_POS.second, 0, heuristic(START_POS.first, START_POS.second), true);
    visited[START_POS.first][START_POS.second] = true;

    // Movement in 8 directions (4 cardinal + 4 diagonal)
    const int dRow[] = {-1, 1, 0, 0, -1, -1, 1, 1};
    const int dCol[] = {0, 0, -1, 1, -1, 1, -1, 1};
    const bool isCardinal[] = {true, true, true, true, false, false, false, false};

    while (!openList.empty()) {
        // Get the node with the lowest f() value (priority queue root)
        Node current = openList.top();
        openList.pop();

        // Check if we reached the end position
        if (current.row == END_POS.first && current.col == END_POS.second) {
            printSolution(maze, &current);
            return;
        }

        // Expand neighbors: prioritize cardinal directions over diagonals
        for (int i = 0; i < 8; ++i) {
            int newRow = current.row + dRow[i];
            int newCol = current.col + dCol[i];
            bool nextMoveIsCardinal = isCardinal[i];

            // Check if the move is valid (within bounds, not a wall, and unvisited)
            if (isValid(newRow, newCol, maze, visited)) {
                visited[newRow][newCol] = true;

                // Prefer cardinal moves by setting a higher gCost for diagonal moves
                int gCost = current.g + (nextMoveIsCardinal ? 1 : 2);
                int hCost = heuristic(newRow, newCol); // Calculate heuristic cost for the new cell
                // Push the new node to the open list with updated costs and parent reference
                openList.emplace(newRow, newCol, gCost, hCost, nextMoveIsCardinal, new Node(current));
            }
        }
    }
}

// Function to read the maze from standard input
void readMazeFromStdin(std::vector<std::string>& maze) {
    std::string line;
    while (std::getline(std::cin, line)) {
        maze.push_back(line);
    }
}

// Function to write the maze to standard output
void writeMazeToStdout(const std::vector<std::string>& maze) {
    for (size_t i = 0; i < maze.size(); ++i) {
        std::cout << maze[i];
        if (i < maze.size() - 1) { // Add newline only between lines
            std::cout << '\n';
        }
    }
}

int main() {
    std::vector<std::string> maze;

    // Read the maze from stdin
    readMazeFromStdin(maze);

    // Solve the maze
    solveMaze(maze);

    // Write the solved maze to stdout
    writeMazeToStdout(maze);

    return 0;
}