#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

// The numbering starts from one, i.e., if A = [3, 1, -1, 2] then
// FindKthLargest(1, A) returns 3, FindKthLargest(2, A) returns 2,
// FindKthLargest(3, A) returns 1, and FindKthLargest(4, A) returns -1.
int FindKthLargest(int k, vector<int>* A_ptr) {
    vector<int>& A = *A_ptr;
    int left = 0, right = A.size() - 1;
    while (left <= right) {
        // select pivot in [left, right]
        int pivot = left + (rand() % (right - left + 1));

        // reorder [elements > pivot | pivot | elements < pivot]
        std::swap(A[right], A[pivot]);
        int insert_idx = left;
        for (int i = left; i < right; ++i) {
            if (A[i] > A[right]) {
                std::swap(A[insert_idx++], A[i]);
            }
        }
        std::swap(A[insert_idx], A[right]);

        // insert_idx is the position of the pivot after the reorder
        if (insert_idx == k - 1) {
            return A[insert_idx];
        } else if (insert_idx < k - 1) {
            left = insert_idx + 1;
        } else {
            right = insert_idx - 1;
        }
    }
    return A[left];
}
int FindKthLargestWrapper(int k, vector<int>& A) {
  return FindKthLargest(k, &A);
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"k", "A"};
  return GenericTestMain(args, "kth_largest_in_array.cc",
                         "kth_largest_in_array.tsv", &FindKthLargestWrapper,
                         DefaultComparator{}, param_names);
}
