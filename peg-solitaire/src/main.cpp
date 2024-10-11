//
// Created by RINI on 12/10/2024.
//

#include <iostream>
#include <vector>
#include "peg-solitaire.h"

int main() {
    Board board;
    std::vector<Move> solution;

    if (solve(board, solution)) {
        // Output the solution in the specified format
        std::cout << "[";
        for (size_t i = 0; i < solution.size(); ++i) {
            Move& move = solution[i];
            // Adjust indices to start at 1
            std::cout << "[[" << move.from.row + 1 << "," << move.from.col + 1 << "],"
                 << "[" << move.removed.row + 1 << "," << move.removed.col + 1 << "],"
                 << "[" << move.to.row + 1 << "," << move.to.col + 1 << "]]";
            if (i != solution.size() -1)
                std::cout << ",";
        }
        std::cout << "]" << std::endl;
    } else {
        std::cout << "No solution found." << std::endl;
    }

    return 0;
}
