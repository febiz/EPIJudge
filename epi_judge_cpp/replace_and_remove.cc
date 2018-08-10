#include <iterator>
#include <string>
#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/timed_executor.h"
using std::string;
using std::vector;

int ReplaceAndRemove(int size, char s[]) {
    int size_after = 0, insert_pos = 0;
    for (int i = 0; i < size; ++i) {
        if (s[i] != 'b') {
            size_after += (s[i] == 'a' ? 2 : 1);
            s[insert_pos++] = s[i];
        }
    }

    int back_insert_pos = size_after-1;
    for (int i = insert_pos-1; i >= 0; --i) {
        if (s[i] == 'a') {
            s[back_insert_pos--] = 'd';
            s[back_insert_pos--] = 'd';
        } else {
            s[back_insert_pos--] = s[i];
        }
    }

    return size_after;
}
vector<string> ReplaceAndRemoveWrapper(TimedExecutor& executor, int size,
                                       const vector<string>& s) {
  std::vector<char> s_copy(s.size(), '\0');
  for (int i = 0; i < s.size(); ++i) {
    if (!s[i].empty()) {
      s_copy[i] = s[i][0];
    }
  }

  int res_size =
      executor.Run([&] { return ReplaceAndRemove(size, s_copy.data()); });

  vector<string> result;
  for (int i = 0; i < res_size; ++i) {
    result.emplace_back(string(1, s_copy[i]));
  }
  return result;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "size", "s"};
  return GenericTestMain(args, "replace_and_remove.cc",
                         "replace_and_remove.tsv", &ReplaceAndRemoveWrapper,
                         DefaultComparator{}, param_names);
}
