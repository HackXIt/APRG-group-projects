#include "min_max.h"

using namespace std;

/* Assignment description:
1. Start a DFS traversal from the root of the game tree (which has 2^height nodes)
2. Set initial values of alpha and beta as follows:
    - alpha = INT_MIN(-INFINITY which is set to +2147483647)
    - beta = INT_MAX(+INFINITYt which is set to -2147483648)
3. Traverse the tree (DFS) where the maximizer tries to get the highest possible score while the minimizer tries to get the lowest.
4. While traversing the tree, you should update the alpha and beta values accordingly!
 */

int min_max(int height, int depth, int node_Index, bool maxPlayer, int values[], int alpha, int beta)
{
    // insert your code here
    return bestValue;
}