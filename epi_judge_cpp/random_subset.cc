#include <algorithm>
#include <functional>
#include <iterator>
#include <numeric>
#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/random_sequence_checker.h"
#include "test_framework/timed_executor.h"
using std::bind;
using std::iota;
using std::vector;
using std::unordered_map;

// Returns a random k-sized subset of {0, 1, ..., n - 1}.
vector<int> RandomSubset(int n, int k) {
    const bool kUseOnSolution = true;
    if (kUseOnSolution) {
        // O(n) time and space
        vector<int> result(n);
        iota(result.begin(), result.end(), 0);

        for (int i = 0; i < k; ++i) {
            int r = rand() % (n-i);
            std::swap(result[i], result[r+i]);
        }
        result.resize(k);
        return result;
    } else {
        // O(k) time and space, only faster if k << n in practice
        unordered_map<int, int> idxMap;
        for (int i = 0; i < k; ++i) {
            int r = rand() % (n-i);
            auto i1 = idxMap.find(i);
            auto i2 = idxMap.find(r+i);
            if (i1 == idxMap.end() && i2 == idxMap.end()) {
                idxMap[i] = r+i;
                idxMap[r+i] = i;
            } else if (i1 != idxMap.end() && i2 == idxMap.end()) {
                // already swapped some j < i with i
                idxMap[r+i] = i1->second;
                i1->second = r+i;
            } else if (i1 == idxMap.end() && i2 != idxMap.end()) {
                // already swapped some j < i with r+i
                idxMap[i] = i2->second;
                i2->second = i;
            } else {
                std::swap(i1->second, i2->second);
            }
        }
        vector<int> result;
        for (int i = 0; i < k; ++i) {
            result.push_back(idxMap[i]);
        }

        return result;
    }
}

bool RandomSubsetRunner(TimedExecutor& executor, int n, int k) {
  vector<vector<int>> results;

  executor.Run([&] {
    std::generate_n(back_inserter(results), 100000,
                    std::bind(RandomSubset, n, k));
  });

  int total_possible_outcomes = BinomialCoefficient(n, k);
  vector<int> A(n);
  iota(begin(A), end(A), 0);
  vector<vector<int>> combinations;
  for (int i = 0; i < BinomialCoefficient(n, k); ++i) {
    combinations.emplace_back(ComputeCombinationIdx(A, n, k, i));
  }
  vector<int> sequence;
  for (vector<int> result : results) {
    sort(begin(result), end(result));
    sequence.emplace_back(
        distance(begin(combinations),
                 find(begin(combinations), end(combinations), result)));
  }
  return CheckSequenceIsUniformlyRandom(sequence, total_possible_outcomes,
                                        0.01);
}

void RandomSubsetWrapper(TimedExecutor& executor, int n, int k) {
  RunFuncWithRetries(bind(RandomSubsetRunner, std::ref(executor), n, k));
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "n", "k"};
  return GenericTestMain(args, "random_subset.cc", "random_subset.tsv",
                         &RandomSubsetWrapper, DefaultComparator{},
                         param_names);
}
