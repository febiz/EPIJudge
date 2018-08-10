#include <vector>
#include "test_framework/generic_test.h"
using std::vector;
bool CanReachEnd(const vector<int>& max_advance_steps) {
    int nsteps = 0, i = 0;
    do {
        nsteps = std::max(max_advance_steps[i++], --nsteps);
    } while (i < max_advance_steps.size() && nsteps);

    return i == max_advance_steps.size();
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"max_advance_steps"};
  return GenericTestMain(args, "advance_by_offsets.cc",
                         "advance_by_offsets.tsv", &CanReachEnd,
                         DefaultComparator{}, param_names);
}
