#include <vector>
#include <unordered_set>
#include "test_framework/generic_test.h"
using std::vector;
using std::unordered_set;
using std::max;

int LongestContainedRange(const vector<int>& A) {
    unordered_set<int> int_set;
    for (int x : A) {
        int_set.insert(x);
    }
    int max_interval_len = std::numeric_limits<int>::min();
    while(!int_set.empty()) {
        int x = *int_set.begin();
        int_set.erase(x);
        int interval_len = 1;
        int y = x;
        while(int_set.count(++y)) {
            int_set.erase(y);
            interval_len++;
        }
        y = x;
        while(int_set.count(--y)) {
            int_set.erase(y);
            interval_len++;
        }
        max_interval_len = max(max_interval_len, interval_len);
    }
    return max_interval_len;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(
      args, "longest_contained_interval.cc", "longest_contained_interval.tsv",
      &LongestContainedRange, DefaultComparator{}, param_names);
}
