#include <vector>
#include "test_framework/generic_test.h"
using std::vector;
vector<int> NextPermutation(vector<int> perm) {
    int i = perm.size()-1;

    do {
        i--;
    } while(i >= 0 && perm[i+1] <= perm[i]); // should be < imo, but some test cases are invalid permutations

    if (i < 0) return {};

    int j = i+1;
    while (perm[j] > perm[i] && j < perm.size()) {
        j++;
    }

    std::swap(perm[i], perm[j-1]);
    std::reverse(perm.begin()+i+1, perm.end());

    return perm;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"perm"};
  return GenericTestMain(args, "next_permutation.cc", "next_permutation.tsv",
                         &NextPermutation, DefaultComparator{}, param_names);
}
