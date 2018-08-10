#include <queue>
#include <vector>
#include "test_framework/generic_test.h"
using std::priority_queue;
using std::vector;
vector<int> SortApproximatelySortedData(
    vector<int>::const_iterator sequence_begin,
    const vector<int>::const_iterator& sequence_end, int k) {
    vector<int> result;
    priority_queue<int, vector<int>, std::greater<int>> min_heap;
    vector<int>::const_iterator it = sequence_begin;
    // fill min heap with up to k+1 elements
    for (int i = 0; i <= k && it != sequence_end; ++i) {
        min_heap.push(*it++);
    }
    while (!min_heap.empty()) {
        result.push_back(min_heap.top()); min_heap.pop();
        if (it != sequence_end) {
            min_heap.push(*it++);
        }
    }
    return result;
}
vector<int> SortApproximatelySortedDataWrapper(const vector<int>& sequence,
                                               int k) {
  return SortApproximatelySortedData(cbegin(sequence), cend(sequence), k);
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"sequence", "k"};
  return GenericTestMain(
      args, "sort_almost_sorted_array.cc", "sort_almost_sorted_array.tsv",
      &SortApproximatelySortedDataWrapper, DefaultComparator{}, param_names);
}
