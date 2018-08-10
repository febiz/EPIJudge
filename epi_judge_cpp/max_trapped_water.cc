#include <vector>
#include "test_framework/generic_test.h"
using std::vector;
using std::max;
using std::min;

int GetMaxTrappedWater(const vector<int>& heights) {
    int i = 0, j = heights.size()-1;
    int max_water = (j-i)*min(heights[i], heights[j]);
    while (i < j) {
        if (heights[i] < heights[j]) {
            i++;
        } else {
            j--;
        }
        max_water = max(max_water, (j-i)*min(heights[i], heights[j]));
    }
    return max_water;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"heights"};
  return GenericTestMain(args, "max_trapped_water.cc", "max_trapped_water.tsv",
                         &GetMaxTrappedWater, DefaultComparator{}, param_names);
}
