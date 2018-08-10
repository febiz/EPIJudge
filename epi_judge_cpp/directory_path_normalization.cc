#include <string>
#include "test_framework/generic_test.h"
using std::string;
string ShortestEquivalentPath(const string& path) {
    std::stack<string> dirs;
    std::istringstream ss(path);
    string curr;
    if (path.empty()) {
        throw(std::invalid_argument("invalid path"));
    }
    if (path[0] == '/') {
        // Absolute dir
        dirs.push("/");
    }
    while (std::getline(ss, curr, '/')) {
        if (curr == ".." && !dirs.empty()) {
            if (dirs.top() != "..") {
                dirs.pop();
            } else if (dirs.top() == "/"){
                throw(std::invalid_argument("invalid path"));
            } else {
                dirs.push(curr);
            }
        } else if (curr != "" && curr != "." ) {
            dirs.push(curr);
        }
    }
    string result;
    while (!dirs.empty()) {
        result = dirs.top() + (result.empty() ? "" : dirs.top() == "/" ? "" : "/") + result;
        dirs.pop();
    }
    return result;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"path"};
  return GenericTestMain(args, "directory_path_normalization.cc",
                         "directory_path_normalization.tsv",
                         &ShortestEquivalentPath, DefaultComparator{},
                         param_names);
}
