#include <string>
#include "test_framework/generic_test.h"
using std::string;

string LookAndSay(int n) {
    string result = "1";
    for (int i = 1; i < n; ++i) {
        string next_result;
        for (int j = 0; j < result.size(); ++j) {
            int k = j+1;
            while (k < result.size() && result[k] == result[j]) {
                k++;
            }
            next_result.push_back('0' + (k-j));
            next_result.push_back(result[j]);
            j = k - 1;
        }
        result = next_result;
    }

    return result;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"n"};
  return GenericTestMain(args, "look_and_say.cc", "look_and_say.tsv",
                         &LookAndSay, DefaultComparator{}, param_names);
}
