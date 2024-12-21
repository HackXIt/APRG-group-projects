//
// Created by RINI on 21/12/2024.
//
#include "gtest/gtest.h"
#include "game_of_life.h"
#include <fstream>

// Test-Suite 1: Individual Logic Tests
struct LogicTestParams {
    int rows;
    int columns;
    std::vector<std::vector<char>> initialState;
    std::vector<std::vector<char>> expectedState;
};

class LogicTest : public ::testing::TestWithParam<LogicTestParams> {};

TEST_P(LogicTest, VerifyUpdateLogic) {
    LogicTestParams params = GetParam();
    GameOfLife game(params.rows, params.columns, params.initialState);
    game.update();
    EXPECT_EQ(game.getGrid(), params.expectedState);
}

INSTANTIATE_TEST_SUITE_P(
    GameOfLifeLogicTests,
    LogicTest,
    ::testing::Values(
        LogicTestParams{3, 3, {{'.', '.', '.'}, {'.', 'x', '.'}, {'.', '.', '.'}}, {{'.', '.', '.'}, {'.', '.', '.'}, {'.', '.', '.'}}},
        LogicTestParams{4, 4, {{'.', '.', '.', '.'}, {'.', 'x', 'x', '.'}, {'.', 'x', 'x', '.'}, {'.', '.', '.', '.'}}, {{'.', '.', '.', '.'}, {'.', 'x', 'x', '.'}, {'.', 'x', 'x', '.'}, {'.', '.', '.', '.'}}}
    )
);

// Test-Suite 2: End-to-End Tests
struct EndToEndTestParams {
    std::string inputFile;
    std::string expectedOutputFile;
};

class EndToEndTest : public ::testing::TestWithParam<EndToEndTestParams> {};

TEST_P(EndToEndTest, VerifyOutputMatchesExpected) {
    EndToEndTestParams params = GetParam();
    GameOfLife game = GameOfLife::fromFile(params.inputFile);
    for (int i = 0; i < 10; ++i) { // Simulate 10 generations
        game.update();
    }

    std::ifstream expectedFile(params.expectedOutputFile);
    ASSERT_TRUE(expectedFile.is_open());

    std::string expectedLine;
    const auto& grid = game.getGrid();
    for (const auto& row : grid) {
        std::string actualLine(row.begin(), row.end());
        std::getline(expectedFile, expectedLine);
        EXPECT_EQ(actualLine, expectedLine);
    }
}

INSTANTIATE_TEST_SUITE_P(
    GameOfLifeEndToEndTests,
    EndToEndTest,
    ::testing::Values(
        EndToEndTestParams{"test_input_1.txt", "test_expected_1.txt"},
        EndToEndTestParams{"test_input_2.txt", "test_expected_2.txt"}
    )
);
