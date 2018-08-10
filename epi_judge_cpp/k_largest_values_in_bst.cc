#include <memory>
#include <vector>
#include "bst_node.h"
#include "test_framework/generic_test.h"
using std::unique_ptr;
using std::vector;

void helper(const unique_ptr<BstNode<int>>& tree, int k, vector<int>& res) {
    if (!tree || res.size() == k) return;
    helper(tree->right, k, res);
    if (res.size() < k) {
        res.push_back(tree->data);
        helper(tree->left, k, res);
    }
}

vector<int> FindKLargestInBST(const unique_ptr<BstNode<int>>& tree, int k) {
    vector<int> result;
    helper(tree, k, result);
    return result;

//    // slower alternative that copies result
//    if (!tree || k == 0) return {};
//    vector<int> result;
//    vector<int> right = FindKLargestInBST(tree->right, k);
//    result.insert(result.end(), right.begin(), right.end());
//    if (result.size() < k) {
//        result.push_back(tree->data);
//        vector<int> left = FindKLargestInBST(tree->left, k-result.size());
//        result.insert(result.end(), left.begin(), left.end());
//    }
//    return result;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree", "k"};
  return GenericTestMain(args, "k_largest_values_in_bst.cc",
                         "k_largest_values_in_bst.tsv", &FindKLargestInBST,
                         &UnorderedComparator<std::vector<int>>, param_names);
}
