//
// Created by RINI on 21/12/2024.
//
#include "gtest/gtest.h"
#include "game_of_life.h"
#include "Timing.h"
#include <fstream>
#include <sstream>
#include <filesystem>

// NOTE https://stackoverflow.com/questions/16491675/how-to-send-custom-message-in-google-c-testing-framework
class TestCout : public std::stringstream
{
public:
    ~TestCout()
    {
        std::cout << "\u001b[32m[          ] \u001b[33m" << str() << "\u001b[0m" << std::flush;
    }
};

#define TEST_COUT  TestCout()

// Test-Suite 1: Individual Logic Tests
struct LogicTestParams {
    int rows;
    int columns;
    std::vector<std::vector<char>> initialState;
    std::vector<std::vector<char>> expectedState;
};

struct LogicMultipleTestParams {
    int rows;
    int columns;
    int generations;
    std::vector<std::vector<char>> initialState;
    std::vector<std::vector<char>> expectedState;
};

class LogicTest : public ::testing::TestWithParam<LogicTestParams> {};

class LogicTestMultipleGenerations : public ::testing::TestWithParam<LogicMultipleTestParams> {};

TEST_P(LogicTest, VerifyUpdateLogic) {
    LogicTestParams params = GetParam();
    GameOfLife game(params.rows, params.columns, params.initialState);
    game.next();
    EXPECT_EQ(game.getGrid(), params.expectedState);
}

TEST_P(LogicTest, VerifyUpdateLogicTiming) {
    LogicTestParams params = GetParam();
    Timing* timing = Timing::getInstance();
    timing->startSetup();
    GameOfLife game(params.rows, params.columns, params.initialState);
    timing->stopSetup();
    timing->startComputation();
    game.next();
    timing->stopComputation();
    timing->startFinalization();
    EXPECT_EQ(game.getGrid(), params.expectedState);
    timing->stopFinalization();
    TEST_COUT << timing->getResults() << std::endl;
}

TEST_P(LogicTestMultipleGenerations, VerifyUpdateLogicMultipleGenerations) {
    LogicMultipleTestParams params = GetParam();
    GameOfLife game(params.rows, params.columns, params.initialState);
    game.update(params.generations);
    EXPECT_EQ(game.getGrid(), params.expectedState);
}

TEST_P(LogicTestMultipleGenerations, VerifyUpdateLogicMultipleGenerationsTiming) {
    LogicMultipleTestParams params = GetParam();
    Timing* timing = Timing::getInstance();
    timing->startSetup();
    GameOfLife game(params.rows, params.columns, params.initialState);
    timing->stopSetup();
    timing->startComputation();
    game.update(params.generations);
    timing->stopComputation();
    timing->startFinalization();
    EXPECT_EQ(game.getGrid(), params.expectedState);
    timing->stopFinalization();
    TEST_COUT << timing->getResults() << std::endl;
}

INSTANTIATE_TEST_SUITE_P(
    GameOfLifeLogicTests,
    LogicTest,
    ::testing::Values(
        LogicTestParams{
            3,
            3,
            {{'.', '.', '.'}, {'.', 'x', '.'}, {'.', '.', '.'}},
            {{'.', '.', '.'}, {'.', '.', '.'}, {'.', '.', '.'}}},
        LogicTestParams{
            4,
            4,
            {{'.', '.', '.', '.'}, {'.', 'x', 'x', '.'}, {'.', 'x', 'x', '.'}, {'.', '.', '.', '.'}},
            {{'.', '.', '.', '.'}, {'.', 'x', 'x', '.'}, {'.', 'x', 'x', '.'}, {'.', '.', '.', '.'}}}
    )
);

INSTANTIATE_TEST_SUITE_P(
    GameOfLifeLogicTests,
    LogicTestMultipleGenerations,
    ::testing::Values(
        LogicMultipleTestParams{
            3,
            3,
            2,
            {{'.', '.', '.'}, {'.', 'x', '.'}, {'.', '.', '.'}},
            {{'.', '.', '.'}, {'.', '.', '.'}, {'.', '.', '.'}}
        },
        LogicMultipleTestParams{
            4,
            4,
            2,
            {{'.', '.', '.', '.'}, {'.', 'x', 'x', '.'}, {'.', 'x', 'x', '.'}, {'.', '.', '.', '.'}},
            {{'.', '.', '.', '.'}, {'.', 'x', 'x', '.'}, {'.', 'x', 'x', '.'}, {'.', '.', '.', '.'}}
        }
    )
);