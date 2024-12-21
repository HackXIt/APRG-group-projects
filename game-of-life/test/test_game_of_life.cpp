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
        EndToEndTestParams{"random250_in.gol", "random250_out.gol"},
        EndToEndTestParams{"random500_in.gol","random500_out.gol"},
        EndToEndTestParams{"random750_in.gol","random750_out"},
        EndToEndTestParams{"random1000_in.gol","random1000_out"},
        EndToEndTestParams{"random1250_in.gol","random1250_out"},
        EndToEndTestParams{"random1500_in.gol","random1500_out"},
        EndToEndTestParams{"random1750_in.gol","random1750_out"},
        EndToEndTestParams{"random2000_in.gol","random2000_out"},
        EndToEndTestParams{"random3000_in.gol","random3000_out"},
        EndToEndTestParams{"random4000_in.gol","random4000_out"},
        EndToEndTestParams{"random5000_in.gol","random5000_out"},
        EndToEndTestParams{"random6000_in.gol","random6000_out"},
        EndToEndTestParams{"random7000_in.gol","random7000_out"},
        EndToEndTestParams{"random8000_in.gol","random8000_out"},
        EndToEndTestParams{"random9000_in.gol","random9000_out"},
        EndToEndTestParams{"random10000_in.gol","random10000_out"},
    )
);
