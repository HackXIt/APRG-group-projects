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

class ScopedFile {
public:
    explicit ScopedFile(const std::string& filename) : file(filename) {
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
    game.next();
    EXPECT_EQ(game.getGrid(), params.expectedState);
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

// Test-Suite 2: End-to-End Tests
struct EndToEndTestParams {
    int generations;
    std::string inputFile;
    std::string expectedOutputFile;
};

class EndToEndTest : public ::testing::TestWithParam<EndToEndTestParams> {
protected:
    static std::string getOutputFilename(const std::string& inputFilename) {
        size_t lastSlash = inputFilename.find_last_of('/');
        std::string baseName = (lastSlash == std::string::npos) ? inputFilename : inputFilename.substr(lastSlash + 1);
        return "output/" + baseName;
    }

    void SetUp() override {
        if (!std::filesystem::exists("output")) {
            std::filesystem::create_directory("output");
        }
    }

    static void compareFiles(const std::string& file1, const std::string& file2) {
        SCOPED_TRACE("Comparing files: " + file1 + " and " + file2);

        ScopedFile stream1(file1);
        ScopedFile stream2(file2);

        // Check lines
        std::string line1, line2;
        while (std::getline(stream1.get(), line1) && std::getline(stream2.get(), line2))
        {
            EXPECT_EQ(line1, line2);
        }
    }
};

TEST_P(EndToEndTest, VerifyOutputMatchesExpected) {
    EndToEndTestParams params = GetParam();

    // Prefix directories
    std::string inputFile = "input/" + params.inputFile;
    std::string expectedFile = "expected/" + params.expectedOutputFile;
    std::string outputFile = getOutputFilename(params.inputFile);

    GameOfLife game = *GameOfLife::fromFile(inputFile);

    game.update(params.generations);

    game.toFile(outputFile);
    compareFiles(outputFile, expectedFile);
}

TEST_P(EndToEndTest, VerifyOutputMatchesExpectedWithTiming)
{
    EndToEndTestParams params = GetParam();

    // Prefix directories
    std::string inputFile = "input/" + params.inputFile;
    std::string expectedFile = "expected/" + params.expectedOutputFile;
    std::string outputFile = getOutputFilename(params.inputFile);

    Timing* timing = Timing::getInstance();
    timing->startSetup();
    GameOfLife game = *GameOfLife::fromFile(inputFile);
    timing->stopSetup();
    timing->startComputation();
    game.update(params.generations);
    timing->stopComputation();
    timing->startFinalization();
    game.toFile(outputFile);
    timing->stopFinalization();

    compareFiles(outputFile, expectedFile);
    TEST_COUT << timing->getResults() << std::endl;
}

INSTANTIATE_TEST_SUITE_P(
    GameOfLifeEndToEndTests,
    EndToEndTest,
    ::testing::Values(
        EndToEndTestParams{250, "random250_in.gol", "random250_out.gol"},
        EndToEndTestParams{250, "random500_in.gol", "random500_out.gol"},
        EndToEndTestParams{250, "random750_in.gol", "random750_out.gol"},
        EndToEndTestParams{250, "random1000_in.gol", "random1000_out.gol"},
        EndToEndTestParams{250, "random1250_in.gol", "random1250_out.gol"},
        EndToEndTestParams{250, "random1500_in.gol", "random1500_out.gol"},
        EndToEndTestParams{250, "random1750_in.gol", "random1750_out.gol"},
        EndToEndTestParams{250, "random2000_in.gol", "random2000_out.gol"},
        EndToEndTestParams{250, "random3000_in.gol", "random3000_out.gol"},
        EndToEndTestParams{250, "random4000_in.gol", "random4000_out.gol"},
        EndToEndTestParams{250, "random5000_in.gol", "random5000_out.gol"},
        EndToEndTestParams{250, "random6000_in.gol", "random6000_out.gol"},
        EndToEndTestParams{250, "random7000_in.gol", "random7000_out.gol"},
        EndToEndTestParams{250, "random8000_in.gol", "random8000_out.gol"},
        EndToEndTestParams{250, "random9000_in.gol", "random9000_out.gol"},
        EndToEndTestParams{250, "random10000_in.gol", "random10000_out.gol"}
    )
);