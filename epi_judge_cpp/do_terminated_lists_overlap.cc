#include <memory>
#include "list_node.h"
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
using std::shared_ptr;

shared_ptr<ListNode<int>> OverlappingNoCycleLists(
    shared_ptr<ListNode<int>> l0, shared_ptr<ListNode<int>> l1) {
    shared_ptr<ListNode<int>> l0iter(l0), l1iter(l1);
    int l0count = 0, l1count = 0;
    while (l0iter && l0iter->next) {
        l0iter = l0iter->next;
        l0count++;
    }
    while (l1iter && l1iter->next) {
        l1iter = l1iter->next;
        l1count++;
    }
    if (l0iter != l1iter) return nullptr;

    l0iter = l0count > l1count ? l0 : l1;
    l1iter = l0count > l1count ? l1 : l0;
    for (int i = 0; i < abs(l0count - l1count); ++i) {
        l0iter = l0iter->next;
    }
    while (l0iter != l1iter) {
        l0iter = l0iter->next;
        l1iter = l1iter->next;
    }
    return l0iter;
}
void OverlappingNoCycleListsWrapper(TimedExecutor& executor,
                                    shared_ptr<ListNode<int>> l0,
                                    shared_ptr<ListNode<int>> l1,
                                    shared_ptr<ListNode<int>> common) {
  if (common) {
    if (l0) {
      auto i = l0;
      while (i->next) {
        i = i->next;
      }
      i->next = common;
    } else {
      l0 = common;
    }

    if (l1) {
      auto i = l1;
      while (i->next) {
        i = i->next;
      }
      i->next = common;
    } else {
      l1 = common;
    }
  }

  auto result = executor.Run([&] { return OverlappingNoCycleLists(l0, l1); });

  if (result != common) {
    throw TestFailure("Invalid result");
  }
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "l0", "l1", "common"};
  return GenericTestMain(
      args, "do_terminated_lists_overlap.cc", "do_terminated_lists_overlap.tsv",
      &OverlappingNoCycleListsWrapper, DefaultComparator{}, param_names);
}
