#include <string>
#include <unordered_map>
#include "test_framework/generic_test.h"
using std::istringstream;
using std::string;
using std::stack;
using std::unordered_map;

bool IsWellFormed(const string& s) {
    unordered_map<char, char> matching = {{'(', ')'}, {'[', ']'}, {'{', '}'}};
    stack<char> checker;
    istringstream ss(s);
    char curr;
    while (ss.get(curr)) {
        if (checker.empty() || matching[checker.top()] != curr) {
            checker.push(curr);
        } else {
            checker.pop();
        }
    }
    return checker.empty();
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"s"};
  return GenericTestMain(args, "is_valid_parenthesization.cc",
                         "is_valid_parenthesization.tsv", &IsWellFormed,
                         DefaultComparator{}, param_names);
}
