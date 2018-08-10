#include <string>
#include <unordered_map>
#include "test_framework/generic_test.h"
using std::string;
using std::unordered_map;

bool IsLetterConstructibleFromMagazine(const string& letter_text,
                                       const string& magazine_text) {
    unordered_map<char, int> char_count;
    for (char c : magazine_text) {
        char_count[c]++;
    }
    for (char c : letter_text) {
        if (char_count[c]-- == 0) {
            return false;
        }
    }
    return true;

//    unordered_map<char, int> char_count;
//    for (char c : magazine_text) {
//        auto search = char_count.find(c);
//        if (search != char_count.end()) {
//            char_count[c]++;
//        } else {
//            char_count[c] = 1;
//        }
//    }
//    for (char c : letter_text) {
//        auto search = char_count.find(c);
//        if (search != char_count.end()) {
//            if (char_count[c] == 0) {
//                return false;
//            }
//            char_count[c]--;
//        } else {
//            return false;
//        }
//    }
//    return true;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"letter_text", "magazine_text"};
  return GenericTestMain(args, "is_anonymous_letter_constructible.cc",
                         "is_anonymous_letter_constructible.tsv",
                         &IsLetterConstructibleFromMagazine,
                         DefaultComparator{}, param_names);
}
