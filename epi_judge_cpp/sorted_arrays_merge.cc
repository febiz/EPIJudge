#include <queue>
#include <vector>
#include "test_framework/generic_test.h"
using std::vector;
using std::priority_queue;

struct Iter {
    Iter(std::vector<int>::const_iterator i1,
         std::vector<int>::const_iterator i2)
        : it(i1), end(i2) {}
    std::vector<int>::const_iterator it, end;
};

struct Compare {
    bool operator() (const Iter& it1, const Iter& it2) const {
        return *(it1.it) > *(it2.it);
    }
};

vector<int> MergeSortedArrays(const vector<vector<int>>& sorted_arrays) {
    priority_queue<Iter, vector<Iter>, Compare> q; // min-heap
    for (int i = 0; i < sorted_arrays.size(); ++i) {
        q.push(Iter(sorted_arrays[i].begin(), sorted_arrays[i].end()));
    }
    vector<int> result;
    while (!q.empty()) {
        // add smallest, advance in array from which smallest was taken
        // and add new element to min-heap (constant size sorted_arrays.size())
        Iter curr = q.top(); q.pop();
        if (curr.it != curr.end) {
            result.push_back(*(curr.it++));
            q.push(curr);
        }
    }
    return result;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"sorted_arrays"};
  return GenericTestMain(args, "sorted_arrays_merge.cc",
                         "sorted_arrays_merge.tsv", &MergeSortedArrays,
                         DefaultComparator{}, param_names);
}
