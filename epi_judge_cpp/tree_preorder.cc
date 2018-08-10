#include <vector>
#include "binary_tree_node.h"
#include "test_framework/generic_test.h"
using std::vector;

vector<int> PreorderTraversal(const unique_ptr<BinaryTreeNode<int>>& tree) {
    vector<int> result;
    if (!tree) return result;

    std::stack<BinaryTreeNode<int>*> s;
    s.push(tree.get());

    while (!s.empty()) {
        BinaryTreeNode<int>* curr = s.top();
        s.pop();
        result.push_back(curr->data);
        if (curr->right) {
            s.push(curr->right.get());
        }
        if (curr->left) {
            s.push(curr->left.get());
        }
    }

    return result;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree"};
  return GenericTestMain(args, "tree_preorder.cc", "tree_preorder.tsv",
                         &PreorderTraversal, DefaultComparator{}, param_names);
}
