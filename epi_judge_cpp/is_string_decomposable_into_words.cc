#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
using std::string;
using std::unordered_set;
using std::vector;

bool helper(const string& domain, const unordered_set<string>& dict,
            unordered_set<string>& invalid_cache, vector<string>& result, int start, int end) {
    if (end == domain.size()) {
        return true;
    }

    if (invalid_cache.count(domain.substr(start)) > 0) {
        return false;
    }

    int pos = end + 1;
    while (pos < domain.size() && dict.count(domain.substr(start, pos - start)) == 0) {
        ++pos;
    }

    if (pos == domain.size()) {
        if (dict.count(domain.substr(start)) > 0) {
            result.push_back(domain.substr(start));
            return true;
        }
        return false;
    }

    result.push_back(domain.substr(start, pos - start));
    if (helper(domain, dict, invalid_cache, result, pos, pos)) {
        return true;
    }
    result.pop_back();
    invalid_cache.insert(domain.substr(pos));

    return helper(domain, dict, invalid_cache, result, start, pos);
}

vector<string> DecomposeIntoDictionaryWords(
    const string& domain, const unordered_set<string>& dictionary) {
    vector<string> result;
    unordered_set<string> invalid_cache;
    bool found = helper(domain, dictionary, invalid_cache, result, 0, 0);
    return found ? result : vector<string>{};
}

void DecomposeIntoDictionaryWordsWrapper(
    TimedExecutor& executor, const string& domain,
    const unordered_set<string>& dictionary, bool decomposable) {
  vector<string> result = executor.Run(
      [&] { return DecomposeIntoDictionaryWords(domain, dictionary); });
  if (!decomposable) {
    if (!result.empty()) {
      throw TestFailure("domain is not decomposable");
    }
    return;
  }

  if (std::any_of(std::begin(result), std::end(result),
                  [&](const std::string& s) { return !dictionary.count(s); })) {
    throw TestFailure("Result uses words not in dictionary");
  }

  if (std::accumulate(std::begin(result), std::end(result), string()) !=
      domain) {
    throw TestFailure("Result is not composed into domain");
  }
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "domain", "dictionary",
                                       "decomposable"};
  return GenericTestMain(args, "is_string_decomposable_into_words.cc",
                         "is_string_decomposable_into_words.tsv",
                         &DecomposeIntoDictionaryWordsWrapper,
                         DefaultComparator{}, param_names);
}
