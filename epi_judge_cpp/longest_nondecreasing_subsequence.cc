#include <vector>
#include "test_framework/generic_test.h"
using std::vector;
using std::max;

int LongestNondecreasingSubsequenceLength(const vector<int>& A) {
    vector<int> L(A.size(), 1);
    int longest_subsequence = 1;
    for (int i = 1; i < A.size(); ++i) {
        for (int j = 0; j < i; ++j) {
            if (A[j] <= A[i]) {
                L[i] = max(L[i], L[j] + 1);
                longest_subsequence = max(longest_subsequence, L[i]);
            }
        }
    }
    return longest_subsequence;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(args, "longest_nondecreasing_subsequence.cc",
                         "longest_nondecreasing_subsequence.tsv",
                         &LongestNondecreasingSubsequenceLength,
                         DefaultComparator{}, param_names);
}
