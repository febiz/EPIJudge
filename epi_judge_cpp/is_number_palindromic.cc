#include "test_framework/generic_test.h"
bool IsPalindromeNumber(int x) {
    if (x < 0) return false;

    // reverse second half and check if equal to first half
    int len = log10(x) + 1;
    int rev = 0;
    for (int i = 0; i < len/2; ++i) {
        rev = 10*rev + x % 10;
        x /= 10;
    }
    // if len is odd ignore central number
    if (len % 2 == 1) {
        x /= 10;
    }

    return x == rev;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x"};
  return GenericTestMain(args, "is_number_palindromic.cc",
                         "is_number_palindromic.tsv", &IsPalindromeNumber,
                         DefaultComparator{}, param_names);
}
