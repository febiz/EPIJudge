#include "binary_tree_with_parent_prototype.h"
#include "test_framework/binary_tree_utils.h"
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"

BinaryTreeNode<int>* LCA(const unique_ptr<BinaryTreeNode<int>>& node0,
                         const unique_ptr<BinaryTreeNode<int>>& node1) {
    // find depth
    int c0 = 0, c1 = 0;
    BinaryTreeNode<int>* iter0 = node0.get(), *iter1 = node1.get();
    while (iter0) {
        iter0 = iter0->parent; c0++;
    }
    while (iter1) {
        iter1 = iter1->parent; c1++;
    }
    // advance deeper node by depth difference
    iter0 = c0 > c1 ? node0.get() : node1.get();
    iter1 = c0 > c1 ? node1.get() : node0.get();
    int diff = abs(c0 - c1);
    while (diff--) {
        iter0 = iter0->parent;
    }
    // when path from node0 to root and node1 to root meet
    // we have found the ancestor
    while (iter0 != iter1) {
        iter0 = iter0->parent;
        iter1 = iter1->parent;
    }
    return iter0;
}
int LcaWrapper(TimedExecutor& executor,
               const unique_ptr<BinaryTreeNode<int>>& tree, int key0,
               int key1) {
  const unique_ptr<BinaryTreeNode<int>>& node0 = MustFindNode(tree, key0);
  const unique_ptr<BinaryTreeNode<int>>& node1 = MustFindNode(tree, key1);

  auto result = executor.Run([&] { return LCA(node0, node1); });

  if (!result) {
    throw TestFailure("Result can not be nullptr");
  }
  return result->data;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "tree", "key0", "key1"};
  return GenericTestMain(args, "lowest_common_ancestor_with_parent.cc",
                         "lowest_common_ancestor.tsv", &LcaWrapper,
                         DefaultComparator{}, param_names);
}
