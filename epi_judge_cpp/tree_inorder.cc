#include <memory>
#include <vector>
#include "binary_tree_node.h"
#include "test_framework/generic_test.h"
using std::unique_ptr;
using std::vector;

vector<int> InorderTraversal(const unique_ptr<BinaryTreeNode<int>>& tree) {
    vector<int> result;
    if (!tree) return result;
    std::stack<BinaryTreeNode<int>*> s;
    s.push(tree.get());
    bool from_right = false;
    while (!s.empty()) {
        BinaryTreeNode<int>* tn = s.top();
        if (!from_right && tn->left) {
            s.push(tn->left.get());
            from_right = false;
        } else {
            result.push_back(tn->data);
            s.pop();
            from_right = true;
            if (tn->right) {
                s.push(tn->right.get());
                from_right = false;
            }
        }
    }
    return result;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree"};
  return GenericTestMain(args, "tree_inorder.cc", "tree_inorder.tsv",
                         &InorderTraversal, DefaultComparator{}, param_names);
}
