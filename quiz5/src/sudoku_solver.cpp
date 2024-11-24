#include "sudoku_solver.h"
#include <sstream>
#include <algorithm>
#include <stdexcept>

Sudoku::Sudoku(const std::string& s) {
    for (int i = 0; i < GRID_SIZE; i++) {
        board[i] = s[i] - '0';
    }
    initializeDomains();
}

bool Sudoku::isSafe(int row, int col, int num) const {
    // Check row
    for (int c = 0; c < SIZE; c++) {
        if (board[row * SIZE + c] == num) {
            return false;
        }
    }

    // Check column
    for (int r = 0; r < SIZE; r++) {
        if (board[r * SIZE + col] == num) {
            return false;
        }
    }

    // Check 3x3 subgrid
    int startRow = (row / 3) * 3;
    int startCol = (col / 3) * 3;
    for (int r = startRow; r < startRow + 3; r++) {
        for (int c = startCol; c < startCol + 3; c++) {
            if (board[r * SIZE + c] == num) {
                return false;
            }
        }
    }

    return true;
}

void Sudoku::initializeDomains() {
    for (int i = 0; i < GRID_SIZE; i++) {
        if (board[i] == 0) {
            domains[i] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        } else {
            domains[i] = {board[i]};
        }
    }
    if (!propagateConstraints()) {
        throw std::runtime_error("Initial Sudoku state is invalid.");
    }
}

bool Sudoku::propagateConstraints() {
    bool updated;
    do {
        updated = false;
        for (int i = 0; i < GRID_SIZE; i++) {
            if (domains[i].size() == 1) {
                int value = *domains[i].begin();
                int row = i / SIZE;
                int col = i % SIZE;

                // Remove value from peers
                for (int c = 0; c < SIZE; c++) {
                    if (c != col) {
                        auto& peerDomain = domains[row * SIZE + c];
                        if (peerDomain.erase(value)) {
                            updated = true;
                        }
                    }
                }
                for (int r = 0; r < SIZE; r++) {
                    if (r != row) {
                        auto& peerDomain = domains[r * SIZE + col];
                        if (peerDomain.erase(value)) {
                            updated = true;
                        }
                    }
                }
                int startRow = (row / 3) * 3;
                int startCol = (col / 3) * 3;
                for (int r = startRow; r < startRow + 3; r++) {
                    for (int c = startCol; c < startCol + 3; c++) {
                        if (r * SIZE + c != i) {
                            auto& peerDomain = domains[r * SIZE + c];
                            if (peerDomain.erase(value)) {
                                updated = true;
                            }
                        }
                    }
                }
            }
        }
    } while (updated);

    // Check for inconsistencies (empty domains)
    for (const auto& domain : domains) {
        if (domain.empty()) {
            return false;
        }
    }

    return true;
}

int Sudoku::selectNextCell() const {
    int minDomainSize = SIZE + 1;
    int selectedCell = -1;

    for (int i = 0; i < GRID_SIZE; i++) {
        if (board[i] == 0 && domains[i].size() < minDomainSize) {
            minDomainSize = domains[i].size();
            selectedCell = i;
        }
    }

    return selectedCell;
}

void Sudoku::restoreDomains(const std::vector<std::pair<int, int>>& changes) {
    for (const auto& [cell, value] : changes) {
        domains[cell].insert(value);
    }
}

bool Sudoku::backtrack() {
    int cell = selectNextCell();
    if (cell == -1) {
        return true; // Solved
    }

    for (int value : domains[cell]) {
        int row = cell / SIZE;
        int col = cell % SIZE;

        if (isSafe(row, col, value)) {
            board[cell] = value;

            // Save changes for backtracking
            std::vector<std::pair<int, int>> changes;
            for (int i = 0; i < GRID_SIZE; i++) {
                if (i != cell && domains[i].erase(value)) {
                    changes.emplace_back(i, value);
                }
            }

            if (propagateConstraints() && backtrack()) {
                return true;
            }

            // Undo changes
            restoreDomains(changes);
            board[cell] = 0;
        }
    }

    return false;
}

void Sudoku::solve() {
    if (!backtrack()) {
        throw std::runtime_error("Sudoku cannot be solved.");
    }
}

void Sudoku::printBoard(std::ostream& os) const {
    os << "done!" << std::endl;
    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            os << board[r * SIZE + c];
            if (c % 3 == 2 && c < SIZE - 1) {
                os << " | ";
            } else if (c < SIZE - 1) {
                os << " ";
            }
        }
        if (r < SIZE - 1) {
            os << '\n';
            if (r % 3 == 2) {
                os << "------+-------+------\n";
            }
        }
    }
}

std::string Sudoku::toString() const {
    std::ostringstream oss;
    for (int i = 0; i < GRID_SIZE; i++) {
        oss << board[i];
    }
    return oss.str();
}