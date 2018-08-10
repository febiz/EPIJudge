#include "binary_tree_node.h"
#include "test_framework/generic_test.h"

int helper(const unique_ptr<BinaryTreeNode<int>>& tree, int prev) {
    int sum = 0;
    if (tree->left) {
        sum += helper(tree->left, 2*prev + tree->data);
    }
    if (tree->right) {
        sum += helper(tree->right, 2*prev + tree->data);
    }
    // sum is 0 for leaf nodes and along paths that are all 0
    return sum ? sum : 2*prev + tree->data;
}

int SumRootToLeaf(const unique_ptr<BinaryTreeNode<int>>& tree) {
    return tree ? helper(tree, 0) : 0;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree"};
  return GenericTestMain(args, "sum_root_to_leaf.cc", "sum_root_to_leaf.tsv",
                         &SumRootToLeaf, DefaultComparator{}, param_names);
}
