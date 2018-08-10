#include <string>
#include <vector>
#include "binary_tree_node.h"
#include "test_framework/generic_test.h"
#include "test_framework/timed_executor.h"
using std::string;
using std::vector;

unique_ptr<BinaryTreeNode<int>> ReconstructPreorder(
    const vector<int*>& preorder) {
    std::stack<unique_ptr<BinaryTreeNode<int>>> s;
    auto it = preorder.rbegin();
    while (it != preorder.rend()) {
        if (*it) {
            // the last two trees on the stack are the children of the new node
            unique_ptr<BinaryTreeNode<int>> tl(nullptr), tr(nullptr);
            if (s.size() >= 2) {
                tl = std::move(s.top()); s.pop();
                tr = std::move(s.top()); s.pop();
            }
            unique_ptr<BinaryTreeNode<int>> node =
                std::make_unique<BinaryTreeNode<int>>(
                        BinaryTreeNode<int>{**it, std::move(tl), std::move(tr)});
            s.push(std::move(node));
        } else {
            s.push(nullptr);
        }
        it++;
    }
    return std::move(s.top());
}
unique_ptr<BinaryTreeNode<int>> ReconstructPreorderWrapper(
    TimedExecutor& executor, const vector<string>& preorder) {
  vector<int> values;
  vector<int*> ptrs;
  values.reserve(preorder.size());
  for (auto& s : preorder) {
    if (s == "null") {
      ptrs.push_back(nullptr);
    } else {
      int i = std::stoi(s);
      values.push_back(i);
      ptrs.push_back(&values.back());
    }
  }

  return executor.Run([&] { return ReconstructPreorder(ptrs); });
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "preorder"};
  return GenericTestMain(args, "tree_from_preorder_with_null.cc",
                         "tree_from_preorder_with_null.tsv",
                         &ReconstructPreorderWrapper, DefaultComparator{},
                         param_names);
}
