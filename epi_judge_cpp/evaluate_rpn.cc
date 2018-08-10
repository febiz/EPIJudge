#include <string>
#include "test_framework/generic_test.h"
using std::string;
using std::stack;
int Evaluate(const string& expression) {
    stack<int> s;
    int pos = 0, nextpos = 0;
    do {
        nextpos = expression.find(',', pos);
        string curr = expression.substr(pos, nextpos-pos);
        if (curr == "+") {
            int val2 = s.top(); s.pop();
            int val1 = s.top(); s.pop();
            s.push(val1 + val2);
        } else if (curr == "-") {
            int val2 = s.top(); s.pop();
            int val1 = s.top(); s.pop();
            s.push(val1 - val2);
        } else if (curr == "*") {
            int val2 = s.top(); s.pop();
            int val1 = s.top(); s.pop();
            s.push(val1 * val2);
        } else if (curr == "/") {
            int val2 = s.top(); s.pop();
            int val1 = s.top(); s.pop();
            s.push(val1 / val2);
        } else {
            s.push(atoi(curr.c_str()));
        }
        pos = nextpos+1;
    } while (nextpos != string::npos);

    return s.top();
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"expression"};
  return GenericTestMain(args, "evaluate_rpn.cc", "evaluate_rpn.tsv", &Evaluate,
                         DefaultComparator{}, param_names);
}
