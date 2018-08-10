#include <memory>
#include "binary_tree_node.h"
#include "test_framework/generic_test.h"
using std::unique_ptr;

bool helper(const unique_ptr<BinaryTreeNode<int>>& tree, int v_min, int v_max) {
    if (!tree) return true;
    if (v_min <= tree->data && v_max >= tree->data) {
        return helper(tree->left, v_min, tree->data) &&
                helper(tree->right, tree->data, v_max);
    } else {
        return false;
    }
}

bool IsBinaryTreeBST(const unique_ptr<BinaryTreeNode<int>>& tree) {
    return helper(tree, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree"};
  return GenericTestMain(args, "is_tree_a_bst.cc", "is_tree_a_bst.tsv",
                         &IsBinaryTreeBST, DefaultComparator{}, param_names);
}
