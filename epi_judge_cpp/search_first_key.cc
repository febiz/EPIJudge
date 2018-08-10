#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

int SearchFirstOfK(const vector<int>& A, int k) {
    int l = 0, u = A.size()-1;
    while (l < u) {
        int m = l + (u - l)/2;
        if (A[m] < k) {
            l = m+1;
        } else {
            u = m;
        }
    }
    return (!A.empty() && A[l] == k) ? l : -1;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A", "k"};
  return GenericTestMain(args, "search_first_key.cc", "search_first_key.tsv",
                         &SearchFirstOfK, DefaultComparator{}, param_names);
}
