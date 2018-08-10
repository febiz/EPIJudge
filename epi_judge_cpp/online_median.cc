#include <queue>
#include <vector>
#include "test_framework/generic_test.h"
using std::priority_queue;
using std::vector;

vector<double> OnlineMedian(vector<int>::const_iterator sequence_begin,
                            const vector<int>::const_iterator& sequence_end) {
    vector<double> result;
    priority_queue<int> max_heap; // <= meadian
    priority_queue<int, vector<int>, std::greater<>> min_heap; // > median

    vector<int>::const_iterator it = sequence_begin;
    if (it != sequence_end) {
        max_heap.push(*it);
        result.push_back(*it++);
    }
    for (; it != sequence_end; ++it) {
        if (*it <= max_heap.top()) {
            max_heap.push(*it);
        } else {
            min_heap.push(*it);
        }
        // rebalance if necessary
        int size_diff = abs(max_heap.size() - min_heap.size());
        if (size_diff > 1) {
            // rebalance
            if (max_heap.size() > min_heap.size()) {
                min_heap.push(max_heap.top());
                max_heap.pop();
            } else {
                max_heap.push(min_heap.top());
                min_heap.pop();
            }
            result.push_back((min_heap.top() + max_heap.top()) / 2.0);
        } else if (size_diff == 1) {
            result.push_back(min_heap.size() > max_heap.size() ? min_heap.top() : max_heap.top());
        } else {
            result.push_back((min_heap.top() + max_heap.top()) / 2.0);
        }
    }
    return result;
}
vector<double> OnlineMedianWrapper(const vector<int>& sequence) {
  return OnlineMedian(cbegin(sequence), cend(sequence));
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"sequence"};
  return GenericTestMain(args, "online_median.cc", "online_median.tsv",
                         &OnlineMedianWrapper, DefaultComparator{},
                         param_names);
}
