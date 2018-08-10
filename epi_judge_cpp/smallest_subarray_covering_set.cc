#include <string>
#include <unordered_set>
#include <vector>
#include <unordered_map>
#include <list>
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
using std::string;
using std::unordered_set;
using std::vector;
using std::unordered_map;
using std::list;

struct Subarray {
    // Represent subarray by starting and ending indices, inclusive.
    int start, end;
    int length() {return abs(end - start);}
    bool operator == (const Subarray& that) {
        return start == that.start && end == that.end;
    }
};

Subarray FindSmallestSubarrayCoveringSet(
    const vector<string> &paragraph, const unordered_set<string> &keywords) {

    list<string> order;

    // initialize search map
    unordered_map<string, std::pair<list<string>::iterator, int>> search_map;
    for (const string& s : keywords) {
        search_map[s] = {order.end(), -1};
    }

    // iterate until all keywords are found once
    int i = 0;
    for (; i < paragraph.size() && order.size() < keywords.size(); ++i) {
        auto search = search_map.find(paragraph[i]);
        if (search != search_map.end()) {
            if (search->second.second != -1) {
                // erase previous occurence
                order.erase(search->second.first);
            }
            // add to back and update index in search_map
            order.push_back(search->first);
            search->second = {std::prev(order.end()), i};
        }
    }

    Subarray start_end = {search_map[order.front()].second,
                          search_map[order.back()].second};

    for (; i < paragraph.size(); ++i) {
        auto search = search_map.find(paragraph[i]);
        if (search != search_map.end()) {
            order.erase(search->second.first);
            order.push_back(search->first);
            search->second = {std::prev(order.end()), i};
            int start = search_map[order.front()].second;
            if (i - start < start_end.length()) {
                start_end = {start, i};
            }
        }
    }
    return start_end;

//    unordered_map<string, int> search_map;
//    for (const string& s : keywords) {
//        search_map[s] = -1;
//    }
//    Subarray init = {int(paragraph.size()), -int(paragraph.size())};
//    Subarray start_end = init;
//    for (int i = 0; i < paragraph.size(); ++i) {
//        auto search = search_map.find(paragraph[i]);
//        if (search != search_map.end()) {
//            // is a keyword
//            search_map[paragraph[i]] = i;
//            Subarray curr_start_end = init;
//            for (auto it : search_map) {
//                if (it.second == -1) {
//                    curr_start_end = init;
//                    break;
//                }
//                curr_start_end = {std::min(curr_start_end.start, it.second),
//                                  std::max(curr_start_end.end, it.second)};
//            }
//            if (abs(curr_start_end.length()) < abs(start_end.length())) {
//                start_end = curr_start_end;
//            }
//        }
//    }
//    return start_end;
}
int FindSmallestSubarrayCoveringSetWrapper(
    TimedExecutor &executor, const vector<string> &paragraph,
    const unordered_set<string> &keywords) {
  unordered_set<string> copy = keywords;

  auto result = executor.Run(
      [&] { return FindSmallestSubarrayCoveringSet(paragraph, keywords); });

  if (result.start < 0 || result.start >= paragraph.size() || result.end < 0 ||
      result.end >= paragraph.size() || result.start > result.end) {
    throw TestFailure("Index out of range");
  }

  for (int i = result.start; i <= result.end; i++) {
    copy.erase(paragraph[i]);
  }

  if (!copy.empty()) {
    throw TestFailure("Not all keywords are in the range");
  }

  return result.end - result.start + 1;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "paragraph", "keywords"};
  return GenericTestMain(args, "smallest_subarray_covering_set.cc",
                         "smallest_subarray_covering_set.tsv",
                         &FindSmallestSubarrayCoveringSetWrapper,
                         DefaultComparator{}, param_names);
}
