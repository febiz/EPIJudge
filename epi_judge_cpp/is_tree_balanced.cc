#include "binary_tree_node.h"
#include "test_framework/generic_test.h"

const int kMaxInt = std::numeric_limits<int>::max();

int helper(const unique_ptr<BinaryTreeNode<int>>& tree) {
    // compute depth of left and right subtree,
    // if difference is greater than 1 the tree
    // is imbalanced.
    if (tree == nullptr) {
        return -1;
    } else {
        int left = helper(tree->left);
        if (left == kMaxInt) {
            return kMaxInt;
        }
        int right = helper(tree->right);
        if (right == kMaxInt || abs(left - right) > 1) {
            return kMaxInt;
        }
        return std::max(left, right) + 1;
    }
}

bool IsBalanced(const unique_ptr<BinaryTreeNode<int>>& tree) {
    return helper(tree) == kMaxInt ? false : true;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree"};
  return GenericTestMain(args, "is_tree_balanced.cc", "is_tree_balanced.tsv",
                         &IsBalanced, DefaultComparator{}, param_names);
}
