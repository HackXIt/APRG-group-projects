//
// Created by RINI on 18/11/2024.
//

#include "sudoku_solver.h"

Sudoku::Sudoku(string s) {
    for (unsigned int i = 0; i < s.length(); i++) {
        board[i] = (int) (s[i] - '0');
    }
}

void Sudoku::solve() {
  // ...
}