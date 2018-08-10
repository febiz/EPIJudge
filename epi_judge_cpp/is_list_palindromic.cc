#include "list_node.h"
#include "test_framework/generic_test.h"

bool IsLinkedListAPalindrome(shared_ptr<ListNode<int>> L) {
    if (!L) return true;

    shared_ptr<ListNode<int>> slow(L), fast(L);
    while (fast && fast->next && fast->next->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    // advance if even, such that fast points to last element of L
    if (fast->next) {
        fast = fast->next;
    }
    // reverse list starting from slow->next
    shared_ptr<ListNode<int>> prev(nullptr), curr(slow->next);
    while (curr) {
        shared_ptr<ListNode<int>> tmp = curr->next;
        curr->next = prev;
        prev = curr;
        curr = tmp;
    }
    // Linked list L is 1 longer if elements are odd, check if fast is nullptr
    while (fast && L->data == fast->data) {
        L = L->next;
        fast = fast->next;
    }
    return !fast;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"L"};
  return GenericTestMain(args, "is_list_palindromic.cc",
                         "is_list_palindromic.tsv", &IsLinkedListAPalindrome,
                         DefaultComparator{}, param_names);
}
