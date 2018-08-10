#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

int NumCombinationsForFinalScore(int final_score,
                                 const vector<int>& individual_play_scores) {
    std::vector<vector<int>> S(individual_play_scores.size(), vector<int>(final_score+1, 0));

    // first column
    for (int i = 0; i < individual_play_scores.size(); ++i) {
        S[i][0] = 1;
    }

    // first row
    for (int j = 1; j <= final_score; ++j) {
        S[0][j] = (j % individual_play_scores[0]) == 0;
    }

    for (int i = 1; i < individual_play_scores.size(); ++i) {
        int s = individual_play_scores[i];
        for (int j = 1; j <= final_score; ++j) {
            // S[i-1][j]: row that uses all scores up to current score
            // (j == s) : add 1 the first time j is a multiple of s
            // S[i][j-s]: contains S[i-1][j-s] + S[i-1][j-2*s] + ... + 1 (if j is a multiple of s)
            S[i][j] += S[i-1][j] + (j - s > 0 ? S[i][j-s] : 0) + (j == s);
        }
    }

    return S.back().back();
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"final_score", "individual_play_scores"};
  return GenericTestMain(args, "number_of_score_combinations.cc",
                         "number_of_score_combinations.tsv",
                         &NumCombinationsForFinalScore, DefaultComparator{},
                         param_names);
}
