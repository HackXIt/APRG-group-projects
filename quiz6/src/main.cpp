#include <iostream>
#include <vector>

#include "min_max.h"

int main(int argc, char* argv[]) {
    int height = -1;
    // Leaves of a binary tree. The last step is always a MAX step
    std::vector<std::vector<int>> list_of_values;
    list_of_values.reserve(8);
    list_of_values.push_back({3,12,2,4,14,2,1,2}); // Expects 4
    list_of_values.push_back({13, 8, 24, -5, 23, 15, -14, -20}); // Expects 13
    list_of_values.push_back({3,8,2,6,5,2,1,2}); // Expects 6
    list_of_values.push_back({-30,1,23,-1,4,7,5,-2,3,12,2,4,14,9,1,11,3,8,21,6,5,26,1,22,13,8,24,-5,23,15}); // Expects 5
    list_of_values.push_back({13, 8, 24, -5, 23, 15, -14, -20}); // Expects 13 (Duplicate)
    list_of_values.push_back({10, 8, 24, -5, 23, 15, -14, -20}); // Expects 10
    list_of_values.push_back({11, 8, 24, -15, 23, 15, -14, -24, 50, 2, 1, 4}); // Expects 11
    list_of_values.push_back({11, 18, 34, -5, 23, 15, -10, -20, -1, 100, 60, 12}); // Expects 18
    for (auto & values : list_of_values)
    {
        height = getHeight(static_cast<int>(values.size())); // Height calculation
        int result = min_max(height, 0, 0, true, &values[0], INT_MIN, INT_MAX);
        std::cout << "Result : " << result << "\n";
    }
    return 0;
}
