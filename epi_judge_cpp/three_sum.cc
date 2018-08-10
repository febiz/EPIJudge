#include <algorithm>
#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

bool HasTwoSum(vector<int>& A, int start, int t) {
    int left = start, right = A.size()-1;
    while (left <= right) {
        if (A[left] + A[right] == t) {
            return true;
        } else if (A[left] + A[right] < t) {
            left++;
        } else {
            right--;
        }
    }
    return false;
}

bool HasThreeSum(vector<int> A, int t) {
    std::sort(A.begin(), A.end());
    for (int i = 0; i < A.size(); ++i) {
        if (HasTwoSum(A, i, t-A[i])) {
            return true;
        }
    }
    return false;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A", "t"};
  return GenericTestMain(args, "three_sum.cc", "three_sum.tsv", &HasThreeSum,
                         DefaultComparator{}, param_names);
}
