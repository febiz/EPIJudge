#include <vector>
#include "test_framework/generic_test.h"
using std::swap;
using std::vector;

bool valid (vector<int>& curr) {
    int last_idx = curr.size() - 1;
    for (int i = 0; i < last_idx; ++i) {
        if (curr[i] == curr[last_idx])
            return false;
    }
    return true;
}

void helper(vector<int>& A, vector<int>& curr, vector<vector<int>>& result) {
    if (curr.size() == A.size()) {
        result.push_back(curr);
    } else {
        for (int x : A) {
            curr.push_back(x);
            if (valid(curr)) {
                helper(A, curr, result);
            }
            curr.pop_back();
        }
    }
}

// Solution EPI 15.3 p. 241
void DirectedPermutations(int i, vector<int> *A_ptr,
                          vector<vector<int>> *result) {
    vector<int> &A = *A_ptr;
    if (i == A.size() - 1) {
        result->emplace_back(A);
        return;
    }

    // Try every possibility for A[i]
    for (int j = i; j < A.size(); ++j) {
        swap(A[i], A[j]);
        // Generate all permutations for A[i + 1 : A.size() - 1].
        DirectedPermutations(i + 1, A_ptr, result);
        swap(A[j], A[i]);
    }
}

vector<vector<int>> Permutations(vector<int> A) {
    vector<vector<int>> result;
    vector<int> curr;
    helper(A, curr, result);
//    DirectedPermutations(0, &A, &result);
    return result;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(
      args, "permutations.cc", "permutations.tsv", &Permutations,
      &UnorderedComparator<vector<vector<int>>>, param_names);
}
