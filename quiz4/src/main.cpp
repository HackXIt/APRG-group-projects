#include "maze_solver.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void readMazeFromFile(const std::string& filename, std::vector<std::string>& maze) {
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        maze.push_back(line);
    }
}

void writeMazeToFile(const std::string& filename, const std::vector<std::string>& maze) {
    std::ofstream file(filename);
    for (size_t i = 0; i < maze.size(); ++i) {
        file << maze[i];
        if (i < maze.size() - 1) { // Add newline only between lines
            file << '\n';
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
        return 1;
    }

    std::string input_file = argv[1];
    std::string output_file = argv[2];

    std::vector<std::string> maze;
    readMazeFromFile(input_file, maze);

    // Solve the maze
    solveMaze(maze);

    // Write the solved maze to the output file
    writeMazeToFile(output_file, maze);

    return 0;
}
