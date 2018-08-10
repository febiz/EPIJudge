#include <vector>
#include "binary_tree_with_parent_prototype.h"
#include "test_framework/generic_test.h"
using std::vector;

vector<int> InorderTraversal(const unique_ptr<BinaryTreeNode<int>>& tree) {
    // geez this was a pita to code
    vector<int> result;
    BinaryTreeNode<int> *curr = tree.get(), *visited = nullptr;
    while (curr) {
        bool left_visited = visited && (visited == curr->left.get());
        bool right_visited = visited && (visited == curr->right.get());
        if (curr->left && !left_visited && !right_visited) {
            // visit left child if no child has been visited yet
            // also check left because:
            //          o
            //         /
            //        o <- need to check left when going up if there is no right child
            //       /
            //      o
            curr = curr->left.get();
        } else if (curr->right && !right_visited) {
            // visit right child if it has not been visited yet
            result.push_back(curr->data);
            curr = curr->right.get();
        } else if (!curr->right && left_visited) {
            // we are coming from the left child but there is no right child
            //          o
            //           \
            //            o <- we are here
            //           /
            //          o
            // add data and continue with parent
            result.push_back(curr->data);
            visited = curr;
            curr = curr->parent;
        } else {
            if (!curr->left && !curr->right) {
                // we are at a leaf node
                result.push_back(curr->data);
            }
            visited = curr;
            curr = curr->parent;
        }
    }
    return result;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree"};
  return GenericTestMain(args, "tree_with_parent_inorder.cc",
                         "tree_with_parent_inorder.tsv", &InorderTraversal,
                         DefaultComparator{}, param_names);
}
