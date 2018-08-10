#include <string>
#include <unordered_map>
#include "test_framework/generic_test.h"
using std::string;
int RomanToInteger(const string& s) {
//    std::unordered_map<char, int> roman_to_int = {{'I', 1}, {'V', 5},
//        {'X', 10}, {'L', 50}, {'C', 100}, {'D', 500}, {'M', 1000}};
//    int result = roman_to_int[s.back()];
//    for (int i = s.size()-2; i >= 0; --i) {
//        if (roman_to_int[s[i]] < roman_to_int[s[i+1]]) {
//            result -= roman_to_int[s[i]];
//        } else {
//            result += roman_to_int[s[i]];
//        }
//    }
//    return result;

    int result = 0;
    for (int i = 0; i < s.size(); ++i) {
        char next = i + 1 < s.size() ? s[i+1] : ' ';
        if (s[i] == 'I') {
            if (next == 'V') {
                result += 4, i++;
            } else if (next == 'X') {
                result += 9, i++;
            } else {
                result++;
            }
        } else if (s[i] == 'V') {
            result += 5;
        } else if (s[i] == 'X') {
            if (next == 'L') {
                result += 40, i++;
            } else if (next == 'C') {
                result += 90, i++;
            } else {
                result += 10;
            }
        } else if (s[i] == 'L') {
            result += 50;
        } else if (s[i] == 'C') {
            if (next == 'D') {
                result +=  400, i++;
            } else if (next == 'M') {
                result += 900, i++;
            } else {
                result += 100;
            }
        } else if (s[i] == 'D') {
            result += 500;
        } else if (s[i] == 'M') {
            result += 1000;
        }
    }
    return result;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"s"};
  return GenericTestMain(args, "roman_to_integer.cc", "roman_to_integer.tsv",
                         &RomanToInteger, DefaultComparator{}, param_names);
}
