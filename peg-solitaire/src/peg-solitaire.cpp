//
// Created by RINI on 12/10/2024.
//

#include <iostream>
#include <vector>
#include "peg-solitaire.h"

using namespace std;

Board::Board() {
    initialize();
}

void Board::initialize() {
    // Initialize the board based on the given initial state
    // '-' represents INVALID
    // 'o' represents PEG
    // '.' represents EMPTY

    int init[Board::SIZE][Board::SIZE] = {
        {-1, -1,  1, 1, 1, -1, -1},
        {-1, -1,  1, 1, 1, -1, -1},
        { 1,  1,  1, 1, 1,  1,  1},
        { 1,  1,  1, 0, 1,  1,  1},
        { 1,  1,  1, 1, 1,  1,  1},
        {-1, -1,  1, 1, 1, -1, -1},
        {-1, -1,  1, 1, 1, -1, -1}
    };

    // Adjust the mapping of init array to cells array
    // Swap rows and columns to match the expected positions
    for (int i = 0; i < Board::SIZE; ++i) {
        for (int j = 0; j < Board::SIZE; ++j) {
            cells[j][i] = static_cast<Cell>(init[i][j]);
        }
    }
}

bool Board::isValidPosition(int row, int col) {
    return row >= 0 && row < SIZE && col >= 0 && col < SIZE && cells[row][col] != INVALID;
}

std::vector<Move> Board::getPossibleMoves() {
    std::vector<Move> moves;

    // Define the four possible directions: up, down, left, right
    // Each direction is represented as a pair of row and column offsets
    int dir[4][2] = {
        {-1, 0}, // Up
        {1, 0},  // Down
        {0, -1}, // Left
        {0, 1}   // Right
    };

    // Iterate over each cell on the board
    for (int i = 0; i < SIZE; ++i) {       // Loop over rows
        for (int j = 0; j < SIZE; ++j) {   // Loop over columns
            // Check if there is a peg at the current position
            if (cells[i][j] == PEG) {
                // For each peg, check all four possible move directions
                for (int d = 0; d < 4; ++d) {
                    // Calculate the position of the adjacent cell in the current direction
                    int ni = i + dir[d][0]; // Adjacent row index
                    int nj = j + dir[d][1]; // Adjacent column index

                    // Calculate the position of the cell two steps away in the same direction
                    int nii = i + 2 * dir[d][0]; // Destination row index
                    int njj = j + 2 * dir[d][1]; // Destination column index

                    // Check if the destination position is valid (within bounds and not an invalid cell)
                    if (isValidPosition(nii, njj)) {
                        // Check if there is a peg in the adjacent cell (the one to be jumped over)
                        if (cells[ni][nj] == PEG) {
                            // Check if the destination cell is empty
                            if (cells[nii][njj] == EMPTY) {
                                // All conditions are met; add this move to the list of possible moves
                                Move move(Position(i, j), Position(ni, nj), Position(nii, njj));
                                moves.push_back(move);
                            }
                        }
                    }
                }
            }
        }
    }

    return moves;
}

bool Board::applyMove(const Move& move) {
    // Move the peg
    cells[move.from.row][move.from.col] = EMPTY;
    cells[move.removed.row][move.removed.col] = EMPTY;
    cells[move.to.row][move.to.col] = PEG;
    return true;
}

void Board::undoMove(const Move& move) {
    // Undo the move
    cells[move.from.row][move.from.col] = PEG;
    cells[move.removed.row][move.removed.col] = PEG;
    cells[move.to.row][move.to.col] = EMPTY;
}

int Board::countPegs() {
    int count = 0;
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            if (cells[i][j] == PEG)
                ++count;
    return count;
}

void Board::printBoard() {
    std::cout << "[";
    for (int i = 0; i < SIZE; ++i) {
        std::cout << "[";
        for (int j = 0; j < SIZE; ++j) {
            char c;
            if (cells[i][j] == INVALID) {
                c = '-';
            } else if (cells[i][j] == EMPTY) {
                c = '.';
            } else if (cells[i][j] == PEG) {
                c = 'o';
            }
            std::cout << c;
            if (j != SIZE - 1) {
                std::cout << ",";
            }
        }
        std::cout << "]";
        if (i != SIZE - 1) {
            std::cout << "," << std::endl;
        }
    }
    std::cout << "]" << std::endl;
}

bool Board::isPegAtCenter() {
    // The center position is at (3,3) in the adjusted board representation
    return cells[3][3] == PEG;
}

bool solve(Board& board, vector<Move>& solution) {
    if (board.countPegs() == 1) {
        if (board.isPegAtCenter()) {
            // Solution found with the last peg at the center
            return true;
        } else {
            // Last peg is not at the center, backtrack
            return false;
        }
    }

    vector<Move> moves = board.getPossibleMoves();

    if (moves.empty())
        return false;

    for (auto& move : moves) {
        board.applyMove(move);
        solution.push_back(move);

        if (solve(board, solution))
            return true;

        solution.pop_back();
        board.undoMove(move);
    }

    return false;
}