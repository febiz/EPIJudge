#include "test_framework/generic_test.h"

double SquareRoot(double x) {
    double l = 0., r = std::max(1., x);
    while (l < r) {
        double m = l + (r - l) / 2.;
        double p = m*m;
        if (fabs(p/x - 1.) <= std::numeric_limits<double>::epsilon()) {
            return m;
        } else if (p < x) {
            l = m;
        } else {
            r = m;
        }
    }
    return l;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x"};
  return GenericTestMain(args, "real_square_root.cc", "real_square_root.tsv",
                         &SquareRoot, DefaultComparator{}, param_names);
}
