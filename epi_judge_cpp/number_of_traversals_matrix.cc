#include "test_framework/generic_test.h"
#include <vector>

using std::vector;

int NumberOfWays(int n, int m) {
    vector<vector<int>> S(n, vector<int>(m, 1));
    for (int i = 1; i < n; ++i) {
        for (int j = 1; j < m; ++j) {
            S[i][j] = S[i-1][j] + S[i][j-1];
        }
    }
    return S[n-1][m-1];
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"n", "m"};
  return GenericTestMain(args, "number_of_traversals_matrix.cc",
                         "number_of_traversals_matrix.tsv", &NumberOfWays,
                         DefaultComparator{}, param_names);
}
