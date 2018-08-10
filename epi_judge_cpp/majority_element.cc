#include <string>
#include <vector>
#include "test_framework/generic_test.h"
using std::string;
using std::vector;

string MajoritySearch(vector<string>::const_iterator stream_begin,
                      const vector<string>::const_iterator stream_end) {

    string element = *stream_begin;
    int count = 1;
    vector<string>::const_iterator it = stream_begin+1;
    for (; it != stream_end; ++it) {
        if (*it != element) {
            if (count == 1) {
                element = *it;
            } else {
                count--;
            }
        } else {
            count++;
        }
    }
    return element;
}
string MajoritySearchWrapper(const vector<string>& stream) {
  return MajoritySearch(cbegin(stream), cend(stream));
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"stream"};
  return GenericTestMain(args, "majority_element.cc", "majority_element.tsv",
                         &MajoritySearchWrapper, DefaultComparator{},
                         param_names);
}
