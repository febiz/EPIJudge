#include "list_node.h"
#include "test_framework/generic_test.h"

shared_ptr<ListNode<int>> ReverseSublist(shared_ptr<ListNode<int>> L, int start,
                                         int finish) {
    if (start == finish) return L;

    shared_ptr<ListNode<int>> dummy(new ListNode<int>({}, L));

    // points to node before start node
    shared_ptr<ListNode<int>> tail_non_reversed(dummy);
    for (int i = 1; i < start; ++i) {
        tail_non_reversed = tail_non_reversed->next;
    }
    // points to start node
    shared_ptr<ListNode<int>> start_node(tail_non_reversed->next);

    // reverse from start to finish
    int count = start;
    shared_ptr<ListNode<int>> prev(nullptr), curr(tail_non_reversed->next);
    while (count++ <= finish) {
        shared_ptr<ListNode<int>> tmp(curr->next);
        curr->next = prev;
        prev = curr;
        curr = tmp;
    }

    // connect start node with node finish + 1 (is null if finish is last node)
    start_node->next = curr;
    // connect node start - 1 to last reversed node
    tail_non_reversed->next = prev;

    return dummy->next;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"L", "start", "finish"};
  return GenericTestMain(args, "reverse_sublist.cc", "reverse_sublist.tsv",
                         &ReverseSublist, DefaultComparator{}, param_names);
}
