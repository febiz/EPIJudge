#include <vector>
#include "test_framework/generic_test.h"
using std::vector;
void ApplyPermutation(vector<int>* perm_ptr, vector<int>* A_ptr) {
    vector<int>& P = *perm_ptr;
    vector<int>& A = *A_ptr;

    if (A.empty()) return;

    vector<bool> isPermuted(A.size(), false);

    for (int i = 0; i < A.size(); ++i) {
        if (!isPermuted[i]) {
            int j = P[i];
            do {
                std::swap(A[i], A[j]);
                isPermuted[j] = true;
                j = P[j];
            } while (j != i);
        }
    }
}

vector<int> ApplyPermutationWrapper(vector<int> perm, vector<int> A) {
  ApplyPermutation(&perm, &A);
  return A;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"perm", "A"};
  return GenericTestMain(args, "apply_permutation.cc", "apply_permutation.tsv",
                         &ApplyPermutationWrapper, DefaultComparator{},
                         param_names);
}
