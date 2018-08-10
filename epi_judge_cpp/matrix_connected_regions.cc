#include <deque>
#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/timed_executor.h"
using std::deque;
using std::vector;

void FlipColor(int x, int y, vector<deque<bool>>* image_ptr) {
    vector<deque<bool>>& img = *image_ptr;
    deque<std::pair<int, int> > coord_queue(1, std::make_pair(x, y));
    const bool kFlipColor = img[x][y];
    while (!coord_queue.empty()) {
        std::pair<int, int> coord = coord_queue.front();
        coord_queue.pop_front();
        img[coord.first][coord.second] = !kFlipColor;
        if (coord.first-1 >= 0 && img[coord.first-1][coord.second] == kFlipColor) {
            coord_queue.push_back({coord.first-1, coord.second});
        }
        if (coord.first+1 < img.size() && img[coord.first+1][coord.second] == kFlipColor) {
            coord_queue.push_back({coord.first+1, coord.second});
        }
        if (coord.second-1 >= 0 && img[coord.first][coord.second-1] == kFlipColor) {
            coord_queue.push_back({coord.first, coord.second-1});
        }
        if (coord.second+1 < img[coord.first].size() && img[coord.first][coord.second+1] == kFlipColor) {
            coord_queue.push_back({coord.first, coord.second+1});
        }
    }
}

vector<vector<int>> FlipColorWrapper(TimedExecutor& executor, int x, int y,
                                     vector<vector<int>> image) {
  vector<deque<bool>> b;
  b.reserve(image.size());
  for (const vector<int>& row : image) {
    deque<bool> tmp;
    tmp.resize(row.size());
    for (int i = 0; i < row.size(); ++i) {
      tmp[i] = static_cast<bool>(row[i]);
    }
    b.push_back(tmp);
  }

  executor.Run([&] { FlipColor(x, y, &b); });

  image.resize(b.size());

  for (int i = 0; i < image.size(); ++i) {
    image[i].resize(b.size());
    for (int j = 0; j < image[i].size(); ++j) {
      image[i][j] = b[i][j];
    }
  }
  return image;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "x", "y", "image"};
  return GenericTestMain(args, "matrix_connected_regions.cc", "painting.tsv",
                         &FlipColorWrapper, DefaultComparator{}, param_names);
}
