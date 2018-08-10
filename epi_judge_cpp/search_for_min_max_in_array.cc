#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
using std::vector;
struct MinMax {
  int smallest, largest;
};

MinMax FindMinMax(const vector<int>& A) {
    int i = 1;
    MinMax result = {std::numeric_limits<int>::max(),
                     std::numeric_limits<int>::min()};
    for (; i < A.size(); i+=2) {
        if (A[i-1] < A[i]) {
            result.smallest = std::min(result.smallest, A[i-1]);
            result.largest = std::max(result.largest, A[i]);
        } else {
            result.largest = std::max(result.largest, A[i-1]);
            result.smallest = std::min(result.smallest, A[i]);
        }
    }
    if (A.size() % 2 != 0) {
        result.largest = std::max(result.largest, A[i-1]);
        result.smallest = std::min(result.smallest, A[i-1]);
    }

    return result;
}
template <>
struct SerializationTraits<MinMax> : UserSerTraits<MinMax, int, int> {};

bool operator==(const MinMax& lhs, const MinMax& rhs) {
  return std::tie(lhs.smallest, lhs.largest) ==
         std::tie(rhs.smallest, rhs.largest);
}

std::ostream& operator<<(std::ostream& out, const MinMax& x) {
  return out << "min: " << x.smallest << ", max: " << x.largest;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(args, "search_for_min_max_in_array.cc",
                         "search_for_min_max_in_array.tsv", &FindMinMax,
                         DefaultComparator{}, param_names);
}
