#include <stdexcept>
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include "test_framework/test_failure.h"
using std::length_error;
using std::max;
using std::shared_ptr;

template <class  T>
struct NodeList {
    NodeList(T val = {}, T max = {}, shared_ptr<NodeList<T>> node = nullptr) : data(val), oldMax(max), next(node) {}
    T data;
    T oldMax;
    std::shared_ptr<NodeList<T>> next;
};

class Stack {
public:
    Stack() : maxVal(std::numeric_limits<int>::min()), head(nullptr) {}
    bool Empty() const {
        return !head;
    }
    int Max() const {
        return maxVal;
    }
    int Pop() {
        int val = 0;
        if (head) {
            val = head->data;
            maxVal = head->oldMax;
            head = head->next;
        }
        return val;
    }
    void Push(int x) {
        shared_ptr<NodeList<int>> newNode = std::make_shared<NodeList<int>>(NodeList<int>{x, maxVal, head});
        head = newNode;
        maxVal = max(maxVal, x);
    }
private:
    shared_ptr<NodeList<int>> head;
    int maxVal;
};
struct StackOp {
  std::string op;
  int argument;
};

template <>
struct SerializationTraits<StackOp> : UserSerTraits<StackOp, std::string, int> {
};

void StackTester(const std::vector<StackOp>& ops) {
  try {
    Stack s;
    for (auto& x : ops) {
      if (x.op == "Stack") {
        continue;
      } else if (x.op == "push") {
        s.Push(x.argument);
      } else if (x.op == "pop") {
        int result = s.Pop();
        if (result != x.argument) {
          throw TestFailure("Pop: expected " + std::to_string(x.argument) +
                            ", got " + std::to_string(result));
        }
      } else if (x.op == "max") {
        int result = s.Max();
        if (result != x.argument) {
          throw TestFailure("Max: expected " + std::to_string(x.argument) +
                            ", got " + std::to_string(result));
        }
      } else if (x.op == "empty") {
        int result = s.Empty();
        if (result != x.argument) {
          throw TestFailure("Empty: expected " + std::to_string(x.argument) +
                            ", got " + std::to_string(result));
        }
      } else {
        throw std::runtime_error("Unsupported stack operation: " + x.op);
      }
    }
  } catch (length_error&) {
    throw TestFailure("Unexpected length_error exception");
  }
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"ops"};
  return GenericTestMain(args, "stack_with_max.cc", "stack_with_max.tsv",
                         &StackTester, DefaultComparator{}, param_names);
}
