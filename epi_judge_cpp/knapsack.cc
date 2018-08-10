#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
using std::vector;
using std::max;

struct Item {
  int weight, value;
  Item operator+ (const Item& other) {
      return {weight + other.weight, value + other.value};
  }
};

int helper(const vector<Item>& items, int capacity, int i) {
    if (i == items.size()-1) {
        return items.back().weight <= capacity ? items.back().value : 0;
    } else {
        if (items[i].weight <= capacity) {
            return max(helper(items, capacity, i+1),
                       items[i].value + helper(items, capacity - items[i].weight, i+1));
        } else {
            return helper(items, capacity, i+1);
        }
    }
}

int OptimumSubjectToCapacity(const vector<Item>& items, int capacity) {
//    return helper(items, capacity, 0);
    vector<vector<int>> C(items.size(), vector<int>(capacity + 1, 0));

    // first row
    for (int j = 1; j <= capacity; ++j) {
        if (items[0].weight <= j) {
            C[0][j] = items[0].value;
        }
    }

    for (int i = 1; i < items.size(); ++i) {
        for (int j = 1; j <= capacity; ++j) {
            if (j - items[i].weight >= 0) {
                C[i][j] = max(C[i-1][j-items[i].weight] + items[i].value, C[i-1][j]);
            } else {
                C[i][j] = C[i-1][j];
            }
        }
    }

    return C.back().back();
}

template <>
struct SerializationTraits<Item> : UserSerTraits<Item, int, int> {};

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"items", "capacity"};
  return GenericTestMain(args, "knapsack.cc", "knapsack.tsv",
                         &OptimumSubjectToCapacity, DefaultComparator{},
                         param_names);
}
