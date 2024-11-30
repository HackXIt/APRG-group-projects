#include "min_max.h"
#include <climits> // For INT_MIN and INT_MAX
#include <cmath>

/* Assignment description:
1. Start a DFS traversal from the root of the game tree (which has 2^height nodes)
2. Set initial values of alpha and beta as follows:
    - alpha = INT_MIN(-INFINITY which is set to +2147483647)
    - beta = INT_MAX(+INFINITYt which is set to -2147483648)
3. Traverse the tree (DFS) where the maximizer tries to get the highest possible score while the minimizer tries to get the lowest.
4. While traversing the tree, you should update the alpha and beta values accordingly!
 */

int getHeight(int n) {
    // Height of a complete binary tree with n leaves is log2(n)
    return static_cast<int>(log2(n));
}

int min_max(int height, int depth, int node_Index, bool maxPlayer, int values[], int alpha, int beta) {
    // Base case: leaf node is reached
    if (depth == height) {
        return values[node_Index];
    }

    if (maxPlayer) {
        int maxEval = INT_MIN;
        // Recur for left and right children
        for (int i = 0; i < height - 1; ++i) {
            int eval = min_max(height, depth + 1, node_Index * 2 + i, false, values, alpha, beta);
            maxEval = (eval > maxEval) ? eval : maxEval;
            alpha = (alpha > eval) ? alpha : eval;
            if (beta <= alpha) {
                break; // Beta cutoff
            }
        }
        return maxEval;
    } else {
        int minEval = INT_MAX;
        // Recur for left and right children
        for (int i = 0; i < height - 1; ++i) {
            int eval = min_max(height, depth + 1, node_Index * 2 + i, true, values, alpha, beta);
            minEval = (eval < minEval) ? eval : minEval;
            beta = (beta < eval) ? beta : eval;
            if (beta <= alpha) {
                break; // Alpha cutoff
            }
        }
        return minEval;
    }
}