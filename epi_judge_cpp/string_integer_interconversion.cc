#include <string>
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
using std::string;

string IntToString(int x) {
    int negative = x < 0 ? 1 : 0;
    int len = (x == 0 ? 0 : log10(fabs(static_cast<double>(x)))) + negative + 1;

    // write sign
    string result(len, '0');
    if (negative == 1) {
        result[0] = '-';
    }

    // write number
    for (int i = len-1; i >= negative; --i) {
        result[i] = abs(x % 10) + '0';
        x /= 10;
    }

    return result;
}

int StringToInt(const string& s) {
    int result = 0;
    int sign = s[0] == '-' ? -1 : 1;
    for (int i = sign < 0 ? 1 : 0; i < s.size(); ++i) {
        result = 10*result + sign*(s[i] - '0');
    }
    return result;
}

void Wrapper(int x, const string& s) {
  if (IntToString(x) != s) {
    throw TestFailure("Int to string conversion failed");
  }

  if (StringToInt(s) != x) {
    throw TestFailure("String to int conversion failed");
  }
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x", "s"};
  return GenericTestMain(args, "string_integer_interconversion.cc",
                         "string_integer_interconversion.tsv", &Wrapper,
                         DefaultComparator{}, param_names);
}
