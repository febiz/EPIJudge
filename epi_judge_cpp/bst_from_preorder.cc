#include <memory>
#include "bst_node.h"
#include "test_framework/generic_test.h"
using std::unique_ptr;
using std::vector;

unique_ptr<BstNode<int>> helper(const vector<int>& v, int s, int e) {
    if (s >= e) {
        return nullptr;
    } else {
        int i = s+1;
        while (i < e && v[i] < v[s]) {
            i++;
        }
        return std::make_unique<BstNode<int>>(v[s], helper(v, s+1, i), helper(v, i, e));
    }
}

unique_ptr<BstNode<int>> RebuildBSTFromPreorder(
    const vector<int>& preorder_sequence) {
    return helper(preorder_sequence, 0, preorder_sequence.size());
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"preorder_sequence"};
  return GenericTestMain(args, "bst_from_preorder.cc", "bst_from_preorder.tsv",
                         &RebuildBSTFromPreorder, DefaultComparator{},
                         param_names);
}
