#include "test_framework/generic_test.h"
unsigned long long ClosestIntSameBitCount(unsigned long long x) {
    // find first two bits that differ and swap them
    int lsb = x & 1;
    int c = 1;
    while ((((x >> c) & 1L) == lsb) && c < 32) {
        c++;
    }
    if (c == 32) {
        // all bits are equal
        throw(std::invalid_argument("x has no closest number with same bit count"));
    }
    return x^((1L << c-1) | (1L << c));
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x"};
  return GenericTestMain(args, "closest_int_same_weight.cc",
                         "closest_int_same_weight.tsv", &ClosestIntSameBitCount,
                         DefaultComparator{}, param_names);
}
