#include <string>
#include <vector>
#include <unordered_map>
#include "test_framework/generic_test.h"
using std::string;
using std::vector;

std::unordered_map<char, vector<string> > letter_lut = {{'0', {"0"}}, {'1', {"1"}},
    {'2', {"A", "B", "C"}}, {'3', {"D", "E", "F"}}, {'4', {"G", "H", "I"}},
    {'5', {"J", "K", "L"}}, {'6', {"M", "N", "O"}}, {'7', {"P", "Q", "R", "S"}},
    {'8', {"T", "U", "V"}}, {'9', {"W", "X", "Y", "Z"}}};

vector<string> PhoneMnemonic(const string& phone_number) {
    if (phone_number.empty()) return {};
    vector<string> result = letter_lut[phone_number[0]];
    for (int i = 1; i < phone_number.size(); ++i) {
        const int curr_size = result.size();
        vector<string> new_letters = letter_lut[phone_number[i]];
        for (int j = 0; j < curr_size; ++j) {
            string curr = result[j];
            result[j] += new_letters[0];
            for (int k = 1; k < new_letters.size(); ++k) {
                result.push_back(curr + new_letters[k]);
            }
        }
    }
    return result;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"phone_number"};
  return GenericTestMain(args, "phone_number_mnemonic.cc",
                         "phone_number_mnemonic.tsv", &PhoneMnemonic,
                         &UnorderedComparator<std::vector<std::string>>,
                         param_names);
}
