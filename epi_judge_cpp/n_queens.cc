#include <algorithm>
#include <iterator>
#include <vector>
#include <unordered_set>
#include "test_framework/generic_test.h"
using std::vector;

bool valid(vector<int>& result) {
    // NOTE: only need to check last added element!
    int last_idx = result.size()-1;
    for (int i = 0; i < last_idx; ++i) {
        if (result[i] == result[last_idx] ||
            abs(result[i] - result[last_idx]) == last_idx - i) {
            return false;
        }
    }
    return true;
}

void helper(vector<int>& curr, vector<vector<int>>& res, int n) {
    if (curr.size() == n) {
        res.push_back(curr);
    } else {
        for (int col = 0; col < n; ++col) {
            curr.push_back(col);
            if (valid(curr)) {
                helper(curr, res, n);
            }
            curr.pop_back();
        }
    }
}

vector<vector<int>> NQueens(int n) {
    vector<vector<int>> result;
    vector<int> curr;
    helper(curr, result, n);
    return result;
}

bool Comp(vector<vector<int>>& a, vector<vector<int>>& b) {
  std::sort(std::begin(a), std::end(a));
  std::sort(std::begin(b), std::end(b));
  return a == b;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"n"};
  return GenericTestMain(args, "n_queens.cc", "n_queens.tsv", &NQueens, &Comp,
                         param_names);
}
