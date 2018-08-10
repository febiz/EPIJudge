#include <string>
#include <unordered_map>
#include "test_framework/generic_test.h"
using std::string;

string ConvertBase(const string& num_as_string, int b1, int b2) {
    std::unordered_map<char, int> char_to_int = {{'0', 0}, {'1', 1}, {'2', 2},
        {'3', 3}, {'4', 4}, {'5', 5}, {'6', 6}, {'7', 7}, {'8', 8}, {'9', 9},
        {'A', 10}, {'B', 11}, {'C', 12}, {'D', 13}, {'E', 14}, {'F', 15}};
    std::array<char, 16> int_to_char = {'0', '1', '2', '3', '4', '5', '6', '7', '8',
                                        '9', 'A', 'B', 'C', 'D', 'E', 'F'};

    if (b1 == b2) return num_as_string;

    // convert from base b1 to base 10
    int num = 0;
    int negative = num_as_string[0] == '-' ? 1 : 0;
    for (int i = negative; i < num_as_string.size(); ++i) {
        num *= b1;
        num += char_to_int[num_as_string[i]];
    }
    if (!num) return num_as_string;

    // convert from base 10 to base b2
    int bpow = pow(b2, int(logf(num) / logf(b2)));
    string result;
    if (negative) result.push_back('-');
    do {
        int coeff = num / bpow;
        if (num - coeff*bpow >= 0) {
            num -= coeff*bpow;
            result.push_back(int_to_char[coeff]);
        } else {
            result.push_back('0');
        }
        bpow /= b2;
    } while (num || bpow > 0);

    return result;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"num_as_string", "b1", "b2"};
  return GenericTestMain(args, "convert_base.cc", "convert_base.tsv",
                         &ConvertBase, DefaultComparator{}, param_names);
}
