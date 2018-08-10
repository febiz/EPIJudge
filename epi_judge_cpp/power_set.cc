#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

vector<vector<int>> GeneratePowerSet(const vector<int>& input_set) {
    vector<vector<int>> result;
    result.push_back({});
    for (int x : input_set) {
        int size = result.size();
        for (int i = 0; i < size; ++i) {
            vector<int> tmp = result[i];
            tmp.push_back(x);
            result.push_back(tmp);
        }
    }
    return result;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"input_set"};
  return GenericTestMain(
      args, "power_set.cc", "power_set.tsv", &GeneratePowerSet,
      &UnorderedComparator<vector<vector<int>>>, param_names);
}
