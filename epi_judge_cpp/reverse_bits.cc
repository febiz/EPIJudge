#include "test_framework/generic_test.h"
unsigned long long ReverseBits(unsigned long long x) {
    // flip x_63 with x_0, x_62 with x_1, etc.
    for (int i = 0, j = 63; i < 32; ++i,--j) {
        // swap bits i and j if different
        if (((x >> i) & 1L) != ((x >> j) & 1L)) {
            x ^= ((1L << i) | (1L << j));
        }
    }
    return x;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x"};
  return GenericTestMain(args, "reverse_bits.cc", "reverse_bits.tsv",
                         &ReverseBits, DefaultComparator{}, param_names);
}
