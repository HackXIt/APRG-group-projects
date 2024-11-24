#include "sudoku_solver.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

void readSudokuFromFile(const std::string& filename, std::vector<std::string>& sudoku) {
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        sudoku.push_back(line);
    }
}

void writeSudokuToFile(const std::string& filename, const std::vector<std::string>& sudoku) {
    std::ofstream file(filename);
    for (size_t i = 0; i < sudoku.size(); ++i) {
        file << sudoku[i];
        if (i < sudoku.size() - 1) { // Add newline only between lines
            file << '\n';
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
        return 1;
    }

    std::string input_file = argv[1];
    std::string output_file = argv[2];

    std::vector<std::string> sudoku;
    readSudokuFromFile(input_file, sudoku);

    // Flatten the input for the Sudoku class
    std::string board = sudoku[0]; // Assuming single-line input
    Sudoku sudokuSolver(board);

    // Solve the Sudoku
    sudokuSolver.solve();

    // Prepare output
    std::ostringstream oss;
    sudokuSolver.print_board(oss);
    std::vector<std::string> solvedSudoku = {oss.str()};

    // Write the solved Sudoku to the output file
    writeSudokuToFile(output_file, solvedSudoku);

    return 0;
}
