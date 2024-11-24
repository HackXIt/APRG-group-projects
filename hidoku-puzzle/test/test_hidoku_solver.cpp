//
// Created by RINI on 06/11/2024.
//
#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono> // For timing
#include <format>

#include "hidoku_solver.h"

// Helper function to read a hidoku puzzle from a single file into a vector of strings
void readHidokuFromFile(const std::string& filename, std::vector<std::string>& hidoku) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return; // Exit the function if the file cannot be opened
    }

    std::string line;
    while (std::getline(file, line)) {
        hidoku.push_back(line);
    }
}

// Helper function to read the content of a single file into a single string for comparison
std::string readFileToString(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return ""; // Return an empty string if the file cannot be opened
    }

    std::ostringstream content;
    content << file.rdbuf(); // Read the entire content of the file into the string stream
    return content.str();
}

// Helper function to generate formatted file names
std::string generateFileName(int index, const std::string& suffix) {
    std::ostringstream fileName;
    fileName << std::setw(3) << std::setfill('0') << index << suffix;
    return fileName.str();
}

// Macro to define test cases dynamically
class HidokuSolverTest : public ::testing::TestWithParam<int> {};

TEST_P(HidokuSolverTest, CorrectOutput) {
    int testIndex = GetParam();

    // Generate file names for the test
    std::string inputFile = "input/" + generateFileName(testIndex, "_problem.txt");
    std::string expectedFile = "expected/" + generateFileName(testIndex, "_solution.txt");

    // Load the input hidoku
    std::vector<std::string> hidoku;
    readHidokuFromFile(inputFile, hidoku);

    // Solve the hidoku in-place
    solveHidoku(hidoku);

    // Convert the solved hidoku to a single string for comparison
    std::ostringstream solvedHidokuStream;
    for (size_t i = 0; i < hidoku.size(); ++i) {
        solvedHidokuStream << hidoku[i];
        if (i < hidoku.size() - 1) { // Add newline only between lines
            solvedHidokuStream << '\n';
        }
    }

    // Load the expected output
    std::string expectedOutput = readFileToString(expectedFile);

    // Compare the solved hidoku to the expected output
    EXPECT_EQ(solvedHidokuStream.str(), expectedOutput) << "Test failed for file: " << inputFile;
}

// Instantiate the test suite with all indices from 1 to 500
INSTANTIATE_TEST_SUITE_P(
    AllHidokuTests,
    HidokuSolverTest,
    ::testing::Range(1, 501) // From 1 to 500 inclusive
);