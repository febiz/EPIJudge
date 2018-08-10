#include <vector>
#include "test_framework/generic_test.h"
using std::vector;
vector<int> Multiply(vector<int> num1, vector<int> num2) {
    if (num1 == vector<int>({0}) || num2 == vector<int>({0}))
        return {0};

    vector<int>& a = num1.size() > num2.size() ? num1 : num2;
    vector<int>& b = num1.size() > num2.size() ? num2 : num1;
    vector<int> result(a.size(), 0);

    int sign = a[0] > 0 ? 1 : -1;
    sign *= b[0] > 0 ? 1 : -1;
    a[0] = abs(a[0]);
    b[0] = abs(b[0]);

    std::reverse(a.begin(), a.end());
    std::reverse(b.begin(), b.end());

    for (int i = 0; i < b.size(); ++i) {
        for (int j = 0; j < a.size(); ++j) {
            if (i+j < result.size()) {
                result[i+j] += a[j]*b[i];
            } else {
                result.push_back(a[j]*b[i]);
            }

            if (result[i+j] > 9) {
                if (i+j+1 < result.size()) {
                    result[i+j+1] += result[i+j]/10;
                } else {
                    result.push_back(result[i+j]/10);
                }

                result[i+j] %= 10;
            }
        }
    }

    std::reverse(result.begin(), result.end());
    result[0] *= sign;

    return result;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"num1", "num2"};
  return GenericTestMain(args, "int_as_array_multiply.cc",
                         "int_as_array_multiply.tsv", &Multiply,
                         DefaultComparator{}, param_names);
}
