#include <vector>
#include "test_framework/generic_test.h"
using std::vector;
vector<int> PlusOne(vector<int> A) {
    int idx = A.size()-1;
    while (A[idx] == 9 && idx >= 0)
        A[idx--] = 0;
    if (idx >= 0)
        A[idx]++;
    else
        A.insert(A.begin(), 1);
    return A;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(args, "int_as_array_increment.cc",
                         "int_as_array_increment.tsv", &PlusOne,
                         DefaultComparator{}, param_names);
}
