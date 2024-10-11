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

    for (int i = 0; i < Board::SIZE; ++i) {
        for (int j = 0; j < Board::SIZE; ++j) {
            cells[i][j] = static_cast<Cell>(init[i][j]);
        }
    }
}

bool Board::isValidPosition(int row, int col) {
    return row >= 0 && row < SIZE && col >= 0 && col < SIZE && cells[row][col] != INVALID;
}

vector<Move> Board::getPossibleMoves() {
    vector<Move> moves;
    // Directions: up, down, left, right
    int dir[4][2] = { {-1,0}, {1,0}, {0,-1}, {0,1} };

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (cells[i][j] == PEG) {
                for (int d = 0; d < 4; ++d) {
                    int ni = i + dir[d][0];
                    int nj = j + dir[d][1];
                    int nii = i + 2 * dir[d][0];
                    int njj = j + 2 * dir[d][1];

                    if (isValidPosition(nii, njj) && cells[ni][nj] == PEG && cells[nii][njj] == EMPTY) {
                        Move move(Position(i, j), Position(ni, nj), Position(nii, njj));
                        moves.push_back(move);
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

bool solve(Board& board, vector<Move>& solution) {
    if (board.countPegs() == 1) {
        // Solution found
        return true;
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