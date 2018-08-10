#include <vector>
#include "binary_tree_node.h"
#include "test_framework/binary_tree_utils.h"
#include "test_framework/generic_test.h"
using std::vector;

unique_ptr<BinaryTreeNode<int>> helper(const vector<int>& preorder,
                                       const vector<int>& inorder,
                                       int i, int j, int k) {
    if (j == k) {
        return nullptr;
    } else {
        // better: use precomputed hashtable to retrieve index
        auto it = std::find(inorder.begin()+j, inorder.begin()+k, preorder[i]);
        int idx = distance(inorder.begin(), it);

        return std::make_unique<BinaryTreeNode<int>>(BinaryTreeNode<int>{preorder[i],
            helper(preorder, inorder, i+1, j, idx),
            helper(preorder, inorder, i+idx-j+1, idx+1, k)});
    }
}


unique_ptr<BinaryTreeNode<int>> BinaryTreeFromPreorderInorder(
    const vector<int>& preorder, const vector<int>& inorder) {
    return helper(preorder, inorder, 0, 0, inorder.size());
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"preorder", "inorder"};
  return GenericTestMain(
      args, "tree_from_preorder_inorder.cc", "tree_from_preorder_inorder.tsv",
      &BinaryTreeFromPreorderInorder, DefaultComparator{}, param_names);
}
