//
// Created by RINI on 21/12/2024.
//
#include "gtest/gtest.h"
#include "game_of_life.h"
#include <fstream>
#include <sstream>
#include <filesystem>

class ScopedFile {
public:
    ScopedFile(const std::string& filename) : file(filename) {
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + filename);
        }
    }

    ~ScopedFile() {
        if (file.is_open()) {
            file.close();
        }
    }

    std::ifstream& get() {
        return file;
    }

private:
    std::ifstream file;
};

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
    int generations;
    std::string inputFile;
    std::string expectedOutputFile;
};

class EndToEndTest : public ::testing::TestWithParam<EndToEndTestParams> {
protected:
    const std::string outputFilename = "test_output.gol";

    void SetUp() override {
        if (std::filesystem::exists(outputFilename)) {
            std::filesystem::remove(outputFilename);
        }
    }

    void TearDown() override {
        if (std::filesystem::exists(outputFilename)) {
            std::filesystem::remove(outputFilename);
        }
    }

    void compareFiles(const std::string& file1, const std::string& file2) {
        SCOPED_TRACE("Comparing files: " + file1 + " and " + file2);

        ScopedFile stream1(file1);
        ScopedFile stream2(file2);

        std::string line1, line2;
        while (std::getline(stream1.get(), line1) && std::getline(stream2.get(), line2)) {
            EXPECT_EQ(line1, line2);
        }

        EXPECT_TRUE(stream1.get().eof() && stream2.get().eof())
            << "Files have different number of lines.";
    }
};

TEST_P(EndToEndTest, VerifyOutputMatchesExpected) {
    EndToEndTestParams params = GetParam();
    GameOfLife game = GameOfLife::fromFile(params.inputFile);

    for (int i = 0; i < params.generations; ++i) {
        game.update();
    }

    game.toFile(outputFilename);
    compareFiles(outputFilename, params.expectedOutputFile);
}

INSTANTIATE_TEST_SUITE_P(
    GameOfLifeEndToEndTests,
    EndToEndTest,
    ::testing::Values(
        EndToEndTestParams{250, "input/random250_in.gol", "expected/random250_out.gol"},
        EndToEndTestParams{250, "input/random500_in.gol", "expected/random500_out.gol"},
        EndToEndTestParams{250, "input/random750_in.gol", "expected/random750_out"},
        EndToEndTestParams{250, "input/random1000_in.gol", "expected/random1000_out"},
        EndToEndTestParams{250, "input/random1250_in.gol", "expected/random1250_out"},
        EndToEndTestParams{250, "input/random1500_in.gol", "expected/random1500_out"},
        EndToEndTestParams{250, "input/random1750_in.gol", "expected/random1750_out"},
        EndToEndTestParams{250, "input/random2000_in.gol", "expected/random2000_out"},
        EndToEndTestParams{250, "input/random3000_in.gol", "expected/random3000_out"},
        EndToEndTestParams{250, "input/random4000_in.gol", "expected/random4000_out"},
        EndToEndTestParams{250, "input/random5000_in.gol", "expected/random5000_out"},
        EndToEndTestParams{250, "input/random6000_in.gol", "expected/random6000_out"},
        EndToEndTestParams{250, "input/random7000_in.gol", "expected/random7000_out"},
        EndToEndTestParams{250, "input/random8000_in.gol", "expected/random8000_out"},
        EndToEndTestParams{250, "input/random9000_in.gol", "expected/random9000_out"},
        EndToEndTestParams{250, "input/random10000_in.gol", "expected/random10000_out"}
    )
);