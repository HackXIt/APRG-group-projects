//
// Created by RINI on 12/10/2024.
//

#ifndef PEG_SOLITAIRE_H
#define PEG_SOLITAIRE_H

#include <vector>

enum Cell {
    INVALID = -1,
    EMPTY = 0,
    PEG = 1
};

struct Position {
    int row;
    int col;
    Position(int r, int c) : row(r), col(c) {}
};

struct Move {
    Position from;
    Position removed;
    Position to;
    Move(Position f, Position r, Position t) : from(f), removed(r), to(t) {}
};

class Board {
public:
    static const int SIZE = 7;
    Cell cells[SIZE][SIZE];
    Board();
    void initialize();
    bool isValidPosition(int row, int col);
    std::vector<Move> getPossibleMoves();
    bool applyMove(const Move& move);
    void undoMove(const Move& move);
    int countPegs();
};

bool solve(Board& board, std::vector<Move>& solution);

#endif // PEG_SOLITAIRE_H

