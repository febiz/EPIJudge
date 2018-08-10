#include <stdexcept>
#include <string>
#include <vector>
#include <queue>
#include "test_framework/generic_test.h"
#include "test_framework/timed_executor.h"
using std::string;
using std::vector;
using std::queue;
using std::pair;

void FillSurroundedRegions(vector<vector<char>>* board_ptr) {
    vector<vector<char>>& board = *board_ptr;

    const char kVisitedWhite = 'X', kCurrVisitedWhite = 'Y';
    // mark border as visited
    for (int i = 0; i < board.size(); ++i) {
        board[i].front() = board[i].front() == 'W' ? kVisitedWhite : board[i].front();
        board[i].back() = board[i].back() == 'W' ? kVisitedWhite : board[i].back();
    }
    for (int j = 0; j < board.front().size(); ++j) {
        board.front()[j] = board.front()[j] == 'W' ? kVisitedWhite : board.front()[j];
    }
    for (int j = 0; j < board.back().size(); ++j) {
        board.back()[j] = board.back()[j] == 'W' ? kVisitedWhite : board.back()[j];
    }

    // BFS for each white node, if reaches visited white mark all as visited white
    // otherwise mark all as black
    for (int i = 1; i < board.size()-1; ++i) {
        for (int j = 1; j < board[i].size()-1; ++j) {
            if (board[i][j] == 'W') {
                queue<pair<int, int>> dfs;
                dfs.push({i,j});
                board[i][j] = kCurrVisitedWhite;
                vector<pair<int, int>> visited_nodes;
                // BFS for a visited white node
                while (!dfs.empty()) {
                    pair<int, int> curr = dfs.front();
                    dfs.pop();
                    visited_nodes.push_back(curr);
                    if (board[curr.first][curr.second] == kVisitedWhite) {
                        // nodes reach border, mark as visited white and clear all
                        for (auto p : visited_nodes) {
                            board[p.first][p.second] = kVisitedWhite;
                        }
                        visited_nodes.clear();
                        while (!dfs.empty()) {
                            pair<int, int> curr = dfs.front();
                            dfs.pop();
                            board[curr.first][curr.second] = kVisitedWhite;
                        }
                        break;
                    }

                    vector<pair<int, int> > dirs = {{-1,0}, {1,0}, {0,-1}, {0,1}};
                    for (auto d : dirs) {
                        int x = curr.first + d.first, y = curr.second + d.second;
                        if (x >= 0 && x < board.size() && y >= 0 && y < board[x].size() &&
                                (board[x][y] == 'W' || board[x][y] == kVisitedWhite)) {
                            dfs.push({x, y});

                            if (board[x][y] == 'W') {
                                // mark as visited in current BFS so that it's added only once
                                board[x][y] = kCurrVisitedWhite;
                            }
                        }
                    }
                }
                if (!visited_nodes.empty()) {
                    // BFS finished without reaching border
                    for (auto p : visited_nodes) {
                        board[p.first][p.second] = 'B';
                    }
                }
            }
        }
    }

    // relabel visited white nodes
    for (int i = 0; i < board.size(); ++i)  {
        for (int j = 0; j < board[i].size(); ++j) {
            if (board[i][j] == kVisitedWhite) {
                board[i][j] = 'W';
            }
        }
    }
}
vector<vector<string>> FillSurroundedRegionsWrapper(
    TimedExecutor& executor, vector<vector<string>> board) {
  vector<vector<char>> char_vector;
  char_vector.resize(board.size());
  for (int i = 0; i < board.size(); i++) {
    for (const string& s : board[i]) {
      if (s.size() != 1) {
        throw std::runtime_error("String size is not 1");
      }
      char_vector[i].push_back(s[0]);
    }
  }

  executor.Run([&] { FillSurroundedRegions(&char_vector); });

  board.clear();
  board.resize(char_vector.size(), {});
  for (int i = 0; i < board.size(); i++) {
    for (char c : char_vector[i]) {
      board[i].emplace_back(1, c);
    }
  }

  return board;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "board"};
  return GenericTestMain(
      args, "matrix_enclosed_regions.cc", "matrix_enclosed_regions.tsv",
      &FillSurroundedRegionsWrapper, DefaultComparator{}, param_names);
}
