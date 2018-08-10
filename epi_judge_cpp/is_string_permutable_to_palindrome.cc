#include <string>
#include <unordered_set>
#include "test_framework/generic_test.h"
using std::string;
using std::unordered_set;

bool CanFormPalindrome(const string& s) {
    unordered_set<char> char_set;
    for (const char& c : s) {
        auto search = char_set.find(c);
        if (search != char_set.end()) {
            char_set.erase(c);
        } else {
            char_set.insert(c);
        }
    }
    return ((s.size() % 2) == 0) ? char_set.size() == 0 : char_set.size() == 1;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"s"};
  return GenericTestMain(args, "is_string_permutable_to_palindrome.cc",
                         "is_string_permutable_to_palindrome.tsv",
                         &CanFormPalindrome, DefaultComparator{}, param_names);
}
