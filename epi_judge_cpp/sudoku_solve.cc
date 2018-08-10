#include <algorithm>
#include <cmath>
#include <iterator>
#include <vector>
#include <unordered_set>
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
using std::begin;
using std::end;
using std::vector;
using std::unordered_set;

bool valid(vector<vector<int>>& sudoku, int i, int j) {
    int val = sudoku[i][j];
    // check row i
    for (int k = 0; k < 9; ++k) {
        int tmp = sudoku[i][k];
        if (k != j && tmp > 0 && tmp == val) {
            return false;
        }
    }
    // check column j
    for (int k = 0; k < 9; ++k) {
        int tmp = sudoku[k][j];
        if (k != i && tmp > 0 && tmp == val) {
            return false;
        }
    }
    // check 3x3
    for (int k = (i/3)*3; k < (i/3)*3 + 3; ++k) {
        for (int l = (j/3)*3; l < (j/3)*3 + 3; ++l) {
            int tmp = sudoku[k][l];
            if (k != i && l != j && val > 0 && tmp == val) {
                return false;
            }
        }
    }
    return true;
}

void print(vector<vector<int>>& sudoku) {
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            std::cout << sudoku[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

bool helper(vector<vector<int>>& sudoku, int i, int j) {
    while (i < 9 && j < 9) {
        if (sudoku[i][j] == 0) {
            for (int v = 1; v < 10; ++v) {
                sudoku[i][j] = v;
                if (valid(sudoku, i, j)) {
                    if (helper(sudoku, (i + 1) % 9, (i == 8) ? j + 1 : j)) {
                        // backpropagate if solution is found
                        return true;
                    }
                }
                sudoku[i][j] = 0;
            }
            // no valid solution found
            return false;
        } else {
            // skip partial assignment
            j = (i == 8) ? j + 1 : j;
            i = (i + 1) % 9;
        }
    }
    return true;
}

bool SolveSudoku(vector<vector<int>>* partial_assignment) {
    vector<vector<int>>& sudoku = *partial_assignment;
    return helper(sudoku, 0, 0);
}

vector<int> GatherColumn(const vector<vector<int>>& data, size_t i) {
  vector<int> result;
  for (auto& row : data) {
    result.push_back(row[i]);
  }
  return result;
}

vector<int> GatherSquareBlock(const vector<vector<int>>& data,
                              size_t block_size, size_t n) {
  vector<int> result;
  size_t block_x = n % block_size;
  size_t block_y = n / block_size;
  for (size_t i = block_x * block_size; i < (block_x + 1) * block_size; i++) {
    for (size_t j = block_y * block_size; j < (block_y + 1) * block_size; j++) {
      result.push_back(data[i][j]);
    }
  }

  return result;
}

void AssertUniqueSeq(const vector<int>& seq) {
  vector<bool> seen(seq.size(), false);
  for (auto& x : seq) {
    if (x == 0) {
      throw TestFailure("Cell left uninitialized");
    }
    if (x < 0 || x > seq.size()) {
      throw TestFailure("Cell value out of range");
    }
    if (seen[x - 1]) {
      throw TestFailure("Duplicate value in section");
    }
    seen[x - 1] = true;
  }
}

void SolveSudokuWrapper(TimedExecutor& executor,
                        const vector<vector<int>>& partial_assignment) {
  vector<vector<int>> solved = partial_assignment;

  executor.Run([&] { SolveSudoku(&solved); });

  if (!std::equal(begin(partial_assignment), end(partial_assignment),
                  begin(solved), end(solved), [](auto br, auto cr) {
                    return std::equal(begin(br), end(br), begin(cr), end(cr),
                                      [](int bcell, int ccell) {
                                        return bcell == 0 || bcell == ccell;
                                      });
                  }))
    throw TestFailure("Initial cell assignment has been changed");

  auto block_size = static_cast<size_t>(sqrt(solved.size()));

  for (size_t i = 0; i < solved.size(); i++) {
    AssertUniqueSeq(solved[i]);
    AssertUniqueSeq(GatherColumn(solved, i));
    AssertUniqueSeq(GatherSquareBlock(solved, block_size, i));
  }
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "partial_assignment"};
  return GenericTestMain(args, "sudoku_solve.cc", "sudoku_solve.tsv",
                         &SolveSudokuWrapper, DefaultComparator{}, param_names);
}
