#include <vector>
#include "test_framework/generic_test.h"
using std::vector;
using std::max;

int CalculateLargestRectangle(const vector<int>& heights) {
    int largest_rectangle = 0;
    for (int i = 0; i < heights.size(); ++i) {
        int left = i-1;
        while (left >= 0 && heights[left] >= heights[i]) {
            left--;
        }
        int right = i+1;
        while (right < heights.size() && heights[right] >= heights[i]) {
            right++;
        }
        largest_rectangle = max(largest_rectangle, heights[i]*(right-left-1));
    }
    return largest_rectangle;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"heights"};
  return GenericTestMain(args, "largest_rectangle_under_skyline.cc",
                         "largest_rectangle_under_skyline.tsv",
                         &CalculateLargestRectangle, DefaultComparator{},
                         param_names);
}
