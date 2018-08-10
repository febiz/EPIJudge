#include <string>
#include <vector>
#include <unordered_map>
#include "test_framework/generic_test.h"
using std::string;
using std::vector;
using std::unordered_map;

int FindNearestRepetition(const vector<string>& paragraph) {
    int nearest = std::numeric_limits<int>::max();
    unordered_map<string, int> str_idx_map;
    for (int i = 0; i < paragraph.size(); ++i) {
        auto search = str_idx_map.find(paragraph[i]);
        if (search != str_idx_map.end()) {
            nearest = std::min(nearest, i - search->second);
        }
        str_idx_map[paragraph[i]] = i;
    }
    return nearest == std::numeric_limits<int>::max() ? -1 : nearest;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"paragraph"};
  return GenericTestMain(args, "nearest_repeated_entries.cc",
                         "nearest_repeated_entries.tsv", &FindNearestRepetition,
                         DefaultComparator{}, param_names);
}
