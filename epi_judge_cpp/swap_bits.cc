#include "test_framework/generic_test.h"
long long SwapBits(long long x, int i, int j) {
    if (i == j) return x;
    long long bi = (x >> i) & 1L, bj = (x >> j) & 1L;
    // check if bit i and j differ, if so xor with 1 to flip bit
    if (bi != bj) {
        x ^= ((1L << i) | (1L << j));
    }
    return x;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x", "i", "j"};
  return GenericTestMain(args, "swap_bits.cc", "swap_bits.tsv", &SwapBits,
                         DefaultComparator{}, param_names);
}
