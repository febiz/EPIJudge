#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <queue>
#include "test_framework/generic_test.h"
using std::string;
using std::unordered_set;
using std::unordered_map;
using std::vector;
using std::queue;

// Uses BFS to find the least steps of transformation.
int TransformString(unordered_set<string> D, const string& s, const string& t) {
//    // build adjacency list
//    unordered_map<string, vector<string>> adj_list;
//    for (auto fit = D.begin(); fit != D.end(); ++fit) {
//        for (auto sit = D.begin(); sit != D.end(); ++sit) {
//            if (*fit != *sit && fit->size() == sit->size()) {
//                int diff_cnt = 0;
//                for (int i = 0; i < fit->size() && diff_cnt <= 1; ++i) {
//                    diff_cnt += (*fit)[i] == (*sit)[i] ? 0 : 1;
//                }
//                if (diff_cnt == 1) {
//                    adj_list[*fit].push_back(*sit);
//                }
//            }
//        }
//    }
    // perform BFS
    int len = -1;
//    unordered_set<string> visited;
    queue<string> q;
    q.push(s);
    D.erase(s);
//    visited.insert(s);
    while (!q.empty()) {
        queue<string> tmp_q;
        len++;
        while (!q.empty()) {
            string curr = q.front();
            q.pop();
            if (curr == t) {
                return len;
            }
            for (int i = 0; i < curr.size(); ++i) {
                char c = curr[i];
                for (int j = 0; j < 26; ++j) {
                    curr[i] = 'a' + j;
                    if (D.find(curr) != D.end()) {
                        D.erase(curr);
                        tmp_q.push(curr);
                    }
                }
                curr[i] = c;
            }
//            for (string s : adj_list[curr]) {
//                if (visited.count(s) == 0) {
//                    tmp_q.push(s);
//                    visited.insert(s);
//                }
//            }
        }
        q = tmp_q;
    }
    return -1;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"D", "s", "t"};
  return GenericTestMain(args, "string_transformability.cc",
                         "string_transformability.tsv", &TransformString,
                         DefaultComparator{}, param_names);
}
