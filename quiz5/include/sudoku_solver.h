#ifndef CSP_ALGORITHM_H
#define CSP_ALGORITHM_H

#include <iostream>
#include <set>
#include <vector>
#include <string>
#include <array>

class Sudoku {
private:
    static const int SIZE = 9;
    static const int GRID_SIZE = 81;
    int board[GRID_SIZE]; // Sudoku board (0 represents empty cells)
    std::array<std::set<int>, GRID_SIZE> domains; // Domains for each cell

    bool isSafe(int row, int col, int num) const;
    void initializeDomains();
    bool propagateConstraints();
    int selectNextCell() const; // Minimum Remaining Values (MRV) heuristic
    bool backtrack();
    void removeFromDomains(int cell, int value);
    void restoreDomains(const std::vector<std::pair<int, int>>& changes);

public:
    Sudoku(const std::string& s);
    void solve();
    void printBoard(std::ostream& os) const;
    std::string toString() const;
};

#endif // CSP_ALGORITHM_H
