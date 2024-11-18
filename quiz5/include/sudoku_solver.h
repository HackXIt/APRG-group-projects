//
// Created by RINI on 18/11/2024.
//

#ifndef CSP_ALGORITHM_H
#define CSP_ALGORITHM_H

#include <iostream>
using namespace std;

class Sudoku {
private:
    int board[82];

public:

    Sudoku(string s);// {
//        for (unsigned int i = 0; i < s.length(); i++) {
//            board[i] = (int) (s[i] - '0');
//        }
//    }

    void solve();// {
        //... put your code here
     //}

};

#endif //CSP_ALGORITHM_H
