//
// Created by RINI on 18/11/2024.
//
#include "gtest/gtest.h"
#include "min_max.h"

// Structure to hold test parameters
struct MinMaxTestParams {
    std::vector<int> values;
    int expected;
};

// Parameterized test class
class MinMaxTest : public ::testing::TestWithParam<MinMaxTestParams> {};

TEST_P(MinMaxTest, CorrectOutput) {
    MinMaxTestParams params = GetParam();
    int* values = params.values.data();
    int size = static_cast<int>(params.values.size());
    int height = getHeight(size);
    int result = min_max(height, 0, 0, true, values, INT_MIN, INT_MAX);
    EXPECT_EQ(result, params.expected);
}

// Test cases
INSTANTIATE_TEST_SUITE_P(
    MinMaxTests,
    MinMaxTest,
    ::testing::Values(
        MinMaxTestParams{ {3, 12, 2, 4, 14, 2, 1, 2}, 4 },
        MinMaxTestParams{ {13, 8, 24, -5, 23, 15, -14, -20}, 13},
        MinMaxTestParams{ {3,8,2,6,5,2,1,2}, 6},
        MinMaxTestParams{ {-30,1,23,-1,4,7,5,-2,3,12,2,4,14,9,1,11,3,8,21,6,5,26,1,22,13,8,24,-5,23,15}, 5},
        MinMaxTestParams{ {13, 8, 24, -5, 23, 15, -14, -20}, 13}, // DUPLICATE
        MinMaxTestParams{ {10, 8, 24, -5, 23, 15, -14, -20}, 10},
        MinMaxTestParams{ {11, 8, 24, -15, 23, 15, -14, -24, 50, 2, 1, 4}, 11},
        MinMaxTestParams{ {11, 18, 34, -5, 23, 15, -10, -20, -1, 100, 60, 12}, 18}
    )
);