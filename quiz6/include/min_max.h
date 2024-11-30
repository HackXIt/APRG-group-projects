//
// Created by RINI on 18/11/2024.
//

#ifndef MIN_MAX_H
#define MIN_MAX_H

#define SIZE(x)  (sizeof(x) / sizeof((x)[0]))

int getHeight(int n);

int min_max(int height, int depth, int node_Index, bool maxPlayer, int values[], int alpha, int beta);

#endif //MIN_MAX_H
