//
// Created by RINI on 06/11/2024.
//
#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono> // For timing
#include "maze_solver.h"

// Helper function to read a maze from a file into a vector of strings
void readMazeFromFile(const std::string& filename, std::vector<std::string>& maze) {
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        maze.push_back(line);
    }
}

// Helper function to read expected output from a file into a single string for comparison
std::string readFileToString(const std::string& filename) {
    std::ifstream file(filename);
    std::ostringstream content;
    content << file.rdbuf();
    return content.str();
}

TEST(MazeSolverTest, CorrectOutput) {
    // Load the input maze
    std::vector<std::string> maze;
    readMazeFromFile("input.txt", maze);

    // Solve the maze in-place
    solveMaze(maze);

    // Convert the solved maze to a single string for comparison
    std::ostringstream solvedMazeStream;
    for (size_t i = 0; i < maze.size(); ++i) {
        solvedMazeStream << maze[i];
        if (i < maze.size() - 1) { // Add newline only between lines
            solvedMazeStream << '\n';
        }
    }

    // Load the expected output
    std::string expectedOutput = readFileToString("expected.txt");

    // Compare the solved maze to the expected output
    EXPECT_EQ(solvedMazeStream.str(), expectedOutput);
}

// Test case for execution time limit
TEST(MazeSolverTest, ExecutionTimeLimit) {
    // Load the input maze
    std::vector<std::string> maze;
    readMazeFromFile("input.txt", maze);

    // Measure execution time
    auto start = std::chrono::high_resolution_clock::now();
    solveMaze(maze);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    // Assert that execution time is under 1 second
    EXPECT_LT(elapsed.count(), 1.0) << "Execution time exceeded 1 second.";
}

// Test case for estimated memory usage (approximate check)
TEST(MazeSolverTest, EstimatedMemoryUsage) {
    // Load the input maze
    std::vector<std::string> maze;
    readMazeFromFile("input.txt", maze);

    // Size estimation of key data structures
    size_t visitedSize = sizeof(bool) * ROWS * COLS;
    size_t mazeSize = 0;
    for (const auto& line : maze) {
        mazeSize += line.size();
    }
    size_t priorityQueueNodeSize = sizeof(Node) * (ROWS * COLS); // Maximum possible nodes in priority queue

    size_t totalEstimatedMemory = visitedSize + mazeSize + priorityQueueNodeSize;

    // Check that estimated memory is within 1 MB (1 MB = 1,024 * 1,024 bytes)
    EXPECT_LT(totalEstimatedMemory, 1 * 1024 * 1024) << "Estimated memory usage exceeded 1 MB.";
}