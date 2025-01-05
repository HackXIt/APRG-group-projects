#include "sudoku_solver.h"

#include <iostream>

Sudoku::Sudoku(const std::string& input) {
    for (int i = 0; i < 81; i++) {
        board[i] = input[i] - '0';
    }
    initialize_neighbors();
    initialize_domains();
}

void Sudoku::initialize_neighbors() {
    for (int i = 0; i < 81; i++) {
        set<int> temp_neighbors;
        int row = i / 9;
        int col = i % 9;
        int box_row = row / 3;
        int box_col = col / 3;

        // Row neighbors
        for (int c = 0; c < 9; c++) {
            int idx = row * 9 + c;
            if (idx != i)
                temp_neighbors.insert(idx);
        }

        // Column neighbors
        for (int r = 0; r < 9; r++) {
            int idx = r * 9 + col;
            if (idx != i)
                temp_neighbors.insert(idx);
        }

        // Box neighbors
        for (int r = box_row * 3; r < box_row * 3 + 3; r++) {
            for (int c = box_col * 3; c < box_col * 3 + 3; c++) {
                int idx = r * 9 + c;
                if (idx != i)
                    temp_neighbors.insert(idx);
            }
        }

        // Copy to neighbors[i]
        neighbors[i] = vector<int>(temp_neighbors.begin(), temp_neighbors.end());
    }
}

void Sudoku::initialize_domains() {
    // Initialize domains to all values
    for (int i = 0; i < 81; i++) {
        if (board[i] != 0) {
            // Assigned cell, domain is only the assigned value
            domains[i].reset();
            domains[i].set(board[i] - 1); // Value v corresponds to bit v-1
        } else {
            // Unassigned cell, domain is all values 1-9
            domains[i].set(); // All bits set
        }
    }

    // For assigned cells, do forward checking
    for (int i = 0; i < 81; i++) {
        if (board[i] != 0) {
            int value = board[i];
            // Remove value from domains of neighbors
            for (int n : neighbors[i]) {
                if (domains[n][value - 1]) {
                    domains[n].reset(value - 1);
                }
            }
        }
    }
}

int Sudoku::select_unassigned_variable() {
    int min_domain_size = 10; // Larger than max domain size
    int var = -1;

    for (int i = 0; i < 81; i++) {
        if (board[i] == 0) {
            int domain_size = domains[i].count();
            if (domain_size < min_domain_size) {
                min_domain_size = domain_size;
                var = i;
                if (domain_size == 1) {
                    break; // Can't get smaller than 1
                }
            }
        }
    }

    return var;
}

bool Sudoku::backtrack() {
    // Check if assignment is complete
    bool complete = true;
    for (int i = 0; i < 81; i++) {
        if (board[i] == 0) {
            complete = false;
            break;
        }
    }
    if (complete) return true;

    // Select unassigned variable with MRV
    int var = select_unassigned_variable();
    if (var == -1) return false; // No unassigned variable found (should not happen)

    // For each value in the domain of var
    for (int v = 1; v <= 9; v++) {
        if (domains[var][v - 1]) {
            // Try assigning value v to var
            board[var] = v;

            // Record domain changes for backtracking
            vector<pair<int, int>> removals;

            // Forward checking
            bool failed = false;
            for (int n : neighbors[var]) {
                if (domains[n][v - 1]) {
                    domains[n].reset(v - 1);
                    removals.push_back(make_pair(n, v - 1));

                    if (domains[n].none()) {
                        // Domain wiped out
                        failed = true;
                        break;
                    }
                }
            }

            if (!failed) {
                // Continue with recursive call
                if (backtrack()) {
                    return true;
                }
            }

            // Restore domains
            for (auto p : removals) {
                domains[p.first].set(p.second);
            }

            // Undo assignment
            board[var] = 0;
        }
    }

    // No value led to a solution
    return false;
}

void Sudoku::print_board(std::ostream& os) const {
    os << "done!" << endl;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            os << board[i * 9 + j];
            if ((j + 1) % 3 == 0 && j != 8) os << " | "; // Print separator between cells
            else if (j != 8) os << " "; // Print space between numbers
        }
        if (i != 8) os << endl;
        if ((i + 1) % 3 == 0 && i != 8) {
            os << "------+-------+------" << endl;
        }
    }
}

void Sudoku::solve() {
    if (backtrack()) {
        print_board(std::cout);
    } else {
        cout << "Cannot solve the puzzle." << endl;
    }
}