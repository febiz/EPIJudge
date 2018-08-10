#include <string>
#include <vector>
#include "test_framework/generic_test.h"
using std::string;
using std::vector;
using std::min;

const int kCostMatch = 1;
const int kCostInsert = 1;
const int kCostDelete = 1;

int LevenshteinDistance(const string& A, const string& B) {
    vector<vector<int>> Cost(A.length()+1, vector<int>(B.length()+1, 0));
    for (int i = 1; i <= A.length(); ++i) {
        Cost[i][0] = Cost[i-1][0] + kCostInsert;
    }
    for (int j = 1; j <= B.length(); ++j) {
        Cost[0][j] = Cost[0][j-1] + kCostDelete;
    }
    for (int i = 1; i <= A.length(); ++i) {
        for (int j = 1; j <= B.length(); ++j) {
            int CostMatch = Cost[i-1][j-1] + (A[i-1] == B[j-1] ? 0 : kCostMatch);
            int CostInsert = Cost[i-1][j] + kCostInsert;
            int CostDelete = Cost[i][j-1] + kCostDelete;
            Cost[i][j] = min({CostMatch, CostInsert, CostDelete});
        }
    }

    return Cost.back().back();
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A", "B"};
  return GenericTestMain(args, "levenshtein_distance.cc",
                         "levenshtein_distance.tsv", &LevenshteinDistance,
                         DefaultComparator{}, param_names);
}
