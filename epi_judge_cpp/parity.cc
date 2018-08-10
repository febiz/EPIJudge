#include "test_framework/generic_test.h"
short Parity(unsigned long long x) {
    int parity = 0;
    while (x) {
        // x    = x_31, ..., x_i, 1, 0, ..., 0
        // x-1  = x_31, ..., x_i, 0, 1, ..., 1
        // &    = x_31, ..., x_i, 0, 0, ..., 0
        x = x & (x-1);
        parity ^= 1;
    }
    return parity;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x"};
  return GenericTestMain(args, "parity.cc", "parity.tsv", &Parity,
                         DefaultComparator{}, param_names);
}
