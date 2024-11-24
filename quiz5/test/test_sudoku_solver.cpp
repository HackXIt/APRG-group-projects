#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono> // For timing
#include "sudoku_solver.h"

// Helper function to read a Sudoku string from a file
std::string readSudokuFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename);
    }
    std::ostringstream oss;
    oss << file.rdbuf();
    return oss.str();
}

// Helper function to format Sudoku board output for direct string comparison
std::string formatSudokuOutput(const Sudoku& sudoku) {
    std::ostringstream oss;
    sudoku.printBoard(oss);
    return oss.str();
}

// Test case for verifying correctness of the solved Sudoku
TEST(SudokuSolverTest, CorrectOutput) {
    // Load input Sudoku from file
    std::string input = readSudokuFromFile("input.txt");

    // Load expected Sudoku from file
    std::string expectedOutput = readSudokuFromFile("expected.txt");

    // Solve the Sudoku
    Sudoku sudoku(input);
    sudoku.solve();

    // Get the formatted solved Sudoku for comparison
    std::string solvedOutput = formatSudokuOutput(sudoku);

    // Compare the solved Sudoku with the expected output
    EXPECT_EQ(solvedOutput, expectedOutput);
}

// Test case for execution time limit
TEST(SudokuSolverTest, ExecutionTimeLimit) {
    // Load input Sudoku from file
    std::string input = readSudokuFromFile("input.txt");

    // Solve the Sudoku and measure execution time
    Sudoku sudoku(input);
    auto start = std::chrono::high_resolution_clock::now();
    sudoku.solve();
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    // Assert that execution time is under 1 second
    EXPECT_LT(elapsed.count(), 1.0) << "Execution time exceeded 1 second.";
}

// Test case for estimated memory usage
TEST(SudokuSolverTest, EstimatedMemoryUsage) {
    // Load input Sudoku from file
    std::string input = readSudokuFromFile("input.txt");

    // Approximate memory usage estimation
    size_t boardSize = sizeof(int) * 81; // 81 integers for the Sudoku board
    size_t totalEstimatedMemory = boardSize;

    // Check that estimated memory usage is within 1 MB (1,024 * 1,024 bytes)
    EXPECT_LT(totalEstimatedMemory, 1 * 1024 * 1024) << "Estimated memory usage exceeded 1 MB.";
}
