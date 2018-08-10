#include <algorithm>
#include <string>
#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
using std::vector;

bool DifferInOneBit(const int a, const int b) {
    int tmp = a ^ b;
    return (tmp != 0 && !(tmp & (tmp - 1)));
}

bool helper(vector<int>& result, vector<bool>& num_taken, int num_bits) {
    if (result.size() == num_taken.size()) {
        return DifferInOneBit(result.front(), result.back());
    } else {
        for (int i = 0; i < num_bits; ++i) {
            int candidate = result.back() ^ (1 << i);
            if (!num_taken[candidate]) {
                result.push_back(candidate);
                num_taken[candidate] = true;
                if (helper(result, num_taken, num_bits)) {
                    return true;
                }
                result.pop_back();
                num_taken[candidate] = false;
            }
        }
        return false;
    }
}

vector<int> GrayCode(int num_bits) {
    vector<int> result({0});
    vector<bool> num_taken(pow(2, num_bits), false);
    num_taken[0] = true;
    helper(result, num_taken, num_bits);
    return result;
}

bool DiffersByOneBit(int x, int y) {
  int bit_difference = x ^ y;
  return bit_difference && !(bit_difference & (bit_difference - 1));
}

void GrayCodeWrapper(TimedExecutor& executor, int num_bits) {
  vector<int> result = executor.Run([&] { return GrayCode(num_bits); });

  int expected_size = (1 << num_bits);
  if (result.size() != expected_size) {
    throw TestFailure("Length mismatch: expected " +
                      std::to_string(expected_size) + ", got " +
                      std::to_string(result.size()));
  }
  for (size_t i = 1; i < result.size(); i++)
    if (!DiffersByOneBit(result[i - 1], result[i])) {
      if (result[i - 1] == result[i]) {
        throw TestFailure("Two adjacent entries are equal");
      } else {
        throw TestFailure("Two adjacent entries differ by more than 1 bit");
      }
    }

  std::sort(begin(result), end(result));
  auto uniq = std::unique(begin(result), end(result));
  if (uniq != end(result)) {
    throw TestFailure("Not all entries are distinct: found " +
                      std::to_string(std::distance(uniq, end(result))) +
                      " duplicates");
  }
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "num_bits"};
  return GenericTestMain(args, "gray_code.cc", "gray_code.tsv",
                         &GrayCodeWrapper, DefaultComparator{}, param_names);
}
