#include <string>
#include <vector>
#include "test_framework/generic_test.h"
using std::string;
using std::vector;

void helper(vector<string>& result, string& curr, int num_left, int num_right, int num_pairs) {
    if (num_left + num_right == 2*num_pairs) {
        result.push_back(curr);
    } else {
        if (num_left < num_pairs) {
            curr[num_left + num_right] = '(';
            helper(result, curr, num_left+1, num_right, num_pairs);
        }
        if (num_right < num_left) {
            curr[num_left + num_right] = ')';
            helper(result, curr, num_left, num_right+1, num_pairs);
        }
    }
}

vector<string> GenerateBalancedParentheses(int num_pairs) {
    vector<string> result;
    string curr(2*num_pairs, '0');
    helper(result, curr, 0, 0, num_pairs);
    return result;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"num_pairs"};
  return GenericTestMain(args, "enumerate_balanced_parentheses.cc",
                         "enumerate_balanced_parentheses.tsv",
                         &GenerateBalancedParentheses,
                         &UnorderedComparator<vector<string>>, param_names);
}
