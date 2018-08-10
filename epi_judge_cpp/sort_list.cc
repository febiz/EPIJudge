#include <memory>
#include "list_node.h"
#include "test_framework/generic_test.h"

shared_ptr<ListNode<int>> StableSortList(shared_ptr<ListNode<int>> L) {
    if (L == nullptr || L->next == nullptr) return L;

    shared_ptr<ListNode<int>> left = std::make_shared<ListNode<int>>(0, nullptr);
    shared_ptr<ListNode<int>> right = std::make_shared<ListNode<int>>(0, nullptr);
    shared_ptr<ListNode<int>> pivot(L), iter(L), liter(left), riter(right);

    // split L in left list with elements < pivot and
    // right list with elements >= pivot
    iter = iter->next;
    while (iter) {
        if (iter->data < pivot->data) {
            liter->next = iter;
            liter = liter->next;
        } else {
            riter->next = iter;
            riter = riter->next;
        }
        iter = iter->next;
    }
    liter->next = nullptr;
    riter->next = nullptr;

    shared_ptr<ListNode<int>> sorted_left = StableSortList(left->next);
    shared_ptr<ListNode<int>> sorted_right = StableSortList(right->next);

    if (sorted_left) {
        // advance to end of sorted left
        liter = sorted_left;
        while (liter->next) {
            liter = liter->next;
        }
        liter->next = pivot;
        pivot->next = sorted_right;
        return sorted_left;
    } else {
        pivot->next = sorted_right;
        return pivot;
    }
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"L"};
  return GenericTestMain(args, "sort_list.cc", "sort_list.tsv", &StableSortList,
                         DefaultComparator{}, param_names);
}
