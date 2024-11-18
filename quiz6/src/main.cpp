#include "min_max.h"

int main(int argc, char* argv[]) {
    int values[] = {3,12,2,4,14,2,1,2}; // leaves of a binary tree. The last step is always a MAX step
    int height = getHeight(SIZE(values)); // in this case 8
    int result = min_max(height, 0, 0, true, values, INT_MIN, INT_MAX);
    cout <<"Result : " << result << "\n";
    return 0;
}
