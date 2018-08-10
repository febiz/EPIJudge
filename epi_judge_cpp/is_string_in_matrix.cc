#include <vector>
#include "test_framework/generic_test.h"
using std::vector;
bool IsPatternContainedInGrid(const vector<vector<int>>& grid,
                              const vector<int>& pattern) {
    for (int i = 0; i < grid.size(); ++i) {
        for (int j = 0; j < grid[0].size(); ++j) {
            int ii = i, jj = j, k = 0;
            for (; k < pattern.size(); ++k) {
                if (ii > 0 && grid[ii-1][jj] == pattern[k]) {
                    ii--;
                } else if (ii < grid.size() && grid[ii+1][jj] == pattern[k]) {
                    ii++;
                } else if (jj > 0 && grid[ii][jj-1] == pattern[k]) {
                    jj--;
                } else if (jj > grid[0].size() && grid[ii][jj+1] == pattern[k]) {
                    jj++;
                } else {
                    break;
                }
            }
            if (k == pattern.size()) {
                return true;
            }
        }
    }
    return false;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"grid", "pattern"};
  return GenericTestMain(args, "is_string_in_matrix.cc",
                         "is_string_in_matrix.tsv", &IsPatternContainedInGrid,
                         DefaultComparator{}, param_names);
}
