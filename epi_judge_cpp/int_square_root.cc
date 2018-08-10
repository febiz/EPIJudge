#include "test_framework/generic_test.h"

int SquareRoot(int k) {
    // this choice of u works for all integers including 0 and 1
    // and avoids overflow when computing u-l+1 for u = INT_MAX
    int l = 0, u = k / 2 + 1;
    while (l < u) {
        long m = l + (u - l + 1) / 2; // round up
        if (m*m > k) {
            u = m - 1;
        } else {
            l = m;
        }
    }
    return l;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"k"};
  return GenericTestMain(args, "int_square_root.cc", "int_square_root.tsv",
                         &SquareRoot, DefaultComparator{}, param_names);
}
