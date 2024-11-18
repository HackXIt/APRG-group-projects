//
// Created by RINI on 18/11/2024.
//
#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono> // For timing
#include "sudoku_solver.h"

// Helper function to read a sudoku from a file into a vector of strings
void readSudokuFromFile(const std::string& filename, std::vector<std::string>& sudoku) {
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        sudoku.push_back(line);
    }
}

// Helper function to read expected output from a file into a single string for comparison
std::string readSudokuToString(const std::string& filename) {
    std::ifstream file(filename);
    std::ostringstream content;
    content << file.rdbuf();
    return content.str();
}

TEST(SudokuSolverTest, CorrectOutput) {
    // Load the input sudoku
    std::vector<std::string> sudoku;
    readSudokuFromFile("input.txt", sudoku);

    // Solve the sudoku in-place
    solveSudoku(sudoku);

    // Convert the solved sudoku to a single string for comparison
    std::ostringstream solvedSudokuStream;
    for (size_t i = 0; i < sudoku.size(); ++i) {
        solvedSudokuStream << sudoku[i];
        if (i < sudoku.size() - 1) { // Add newline only between lines
            solvedSudokuStream << '\n';
        }
    }

    // Load the expected output
    std::string expectedOutput = readFileToString("expected.txt");

    // Compare the solved sudoku to the expected output
    EXPECT_EQ(solvedSudokuStream.str(), expectedOutput);
}

// Test case for execution time limit
TEST(SudokuSolverTest, ExecutionTimeLimit) {
    // Load the input sudoku
    std::vector<std::string> sudoku;
    readSudokuFromFile("input.txt", sudoku);

    // Measure execution time
    auto start = std::chrono::high_resolution_clock::now();
    solveSudoku(sudoku);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    // Assert that execution time is under 1 second
    EXPECT_LT(elapsed.count(), 1.0) << "Execution time exceeded 1 second.";
}

// Test case for estimated memory usage (approximate check)
TEST(SudokuSolverTest, EstimatedMemoryUsage) {
    // Load the input sudoku
    std::vector<std::string> sudoku;
    readSudokuFromFile("input.txt", sudoku);

    // Size estimation of key data structures
    size_t visitedSize = sizeof(bool) * ROWS * COLS;
    size_t sudokuSize = 0;
    for (const auto& line : sudoku) {
        sudokuSize += line.size();
    }
    size_t priorityQueueNodeSize = sizeof(Node) * (ROWS * COLS); // Maximum possible nodes in priority queue

    size_t totalEstimatedMemory = visitedSize + sudokuSize + priorityQueueNodeSize;

    // Check that estimated memory is within 1 MB (1 MB = 1,024 * 1,024 bytes)
    EXPECT_LT(totalEstimatedMemory, 1 * 1024 * 1024) << "Estimated memory usage exceeded 1 MB.";
}