#ifndef CSP_ALGORITHM_H
#define CSP_ALGORITHM_H

#include <string>
#include <bitset>
#include <vector>
#include <set>
using namespace std;

class Sudoku {
public:
    explicit Sudoku(const std::string& input);

    void solve();
    void print_board(std::ostream& os) const;
private:
    int board[81]{}; // Current assignment of the Sudoku board
    bitset<9> domains[81]; // Domains for each variable (cell)
    vector<int> neighbors[81]; // Neighbors for each cell

    void initialize_neighbors();

    void initialize_domains();

    int select_unassigned_variable();

    bool backtrack();
};

#endif // CSP_ALGORITHM_H
