#include <memory>
#include <vector>
#include "binary_tree_node.h"
#include "test_framework/generic_test.h"
using std::unique_ptr;
using std::vector;

vector<vector<int>> BinaryTreeDepthOrder(
    const unique_ptr<BinaryTreeNode<int>>& tree) {
    std::queue<BinaryTreeNode<int>*> q;
    vector<vector<int>> result;
    if (!tree) return {};
    q.push(tree.get());
    result.push_back(vector<int>(1, tree->data));
    while (!q.empty()) {
        vector<BinaryTreeNode<int>*> tmp;
        while (!q.empty()) {
            if (q.front()->left) {
                tmp.push_back(q.front()->left.get());
            }
            if (q.front()->right) {
                tmp.push_back(q.front()->right.get());
            }
            q.pop();
        }
        if (!tmp.empty()) {
            vector<int> curr;
            for (BinaryTreeNode<int>* node : tmp) {
                q.push(node);
                curr.push_back(node->data);
            }
            result.push_back(curr);
        }
    }
    return result;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree"};
  return GenericTestMain(args, "tree_level_order.cc", "tree_level_order.tsv",
                         &BinaryTreeDepthOrder, DefaultComparator{},
                         param_names);
}
