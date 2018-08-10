#include "list_node.h"
#include "test_framework/generic_test.h"
shared_ptr<ListNode<int>> EvenOddMerge(const shared_ptr<ListNode<int>>& L) {
    shared_ptr<ListNode<int>> dummy = make_shared<ListNode<int>>(ListNode<int>{0, L});
    shared_ptr<ListNode<int>> odd(dummy), even(L);
    while (even && odd && odd->next) {
        odd->next = even->next;
        odd = odd->next;
        if (odd && odd->next) {
            even->next = odd->next;
            even = even->next;
        }
    }
    if (even) {
        even->next = dummy->next;
    }
    return L;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"L"};
  return GenericTestMain(args, "even_odd_list_merge.cc",
                         "even_odd_list_merge.tsv", &EvenOddMerge,
                         DefaultComparator{}, param_names);
}
