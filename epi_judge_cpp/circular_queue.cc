#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include "test_framework/test_failure.h"
using std::vector;

class Queue {
public:
    Queue(size_t capacity)
        : data_(vector<int>(capacity)), first_(0), last_(0), num_elements_(0)
    {}

    void Enqueue(int x) {
        if (data_.size() == num_elements_) {
            // first_ == last_, copy [0,last_) to [num_elements_, num_elements_+last_)
            data_.resize(2*data_.size());
            std::copy(data_.begin(), data_.begin()+last_, data_.begin()+num_elements_);
            last_ = (first_ + num_elements_) % data_.size();
        }
        data_[last_] = x;
        last_ = (last_ + 1) % data_.size();
        num_elements_++;
    }

    int Dequeue() {
        if (num_elements_ == 0) {
            throw(std::length_error("Dequeue on empty queue."));
        }
        int val = data_[first_];
        first_ = (first_ + 1) % data_.size();
        num_elements_--;
        return val;
    }

    int Size() const {
        return num_elements_;
    }

    private:
        vector<int> data_;
        int first_;
        int last_;
        int num_elements_;
};

struct QueueOp {
  enum { kConstruct, kDequeue, kEnqueue, kSize } op;
  int argument;

  QueueOp(const std::string& op_string, int arg) : argument(arg) {
    if (op_string == "Queue") {
      op = kConstruct;
    } else if (op_string == "dequeue") {
      op = kDequeue;
    } else if (op_string == "enqueue") {
      op = kEnqueue;
    } else if (op_string == "size") {
      op = kSize;
    } else {
      throw std::runtime_error("Unsupported queue operation: " + op_string);
    }
  }

  void execute(Queue& q) const {
    switch (op) {
      case kConstruct:
        // Hack to bypass deleted assign operator
        q.~Queue();
        new (&q) Queue(argument);
        break;
      case kDequeue: {
        int result = q.Dequeue();
        if (result != argument) {
          throw TestFailure("Dequeue: expected " + std::to_string(argument) +
                            ", got " + std::to_string(result));
        }
      } break;
      case kEnqueue:
        q.Enqueue(argument);
        break;
      case kSize: {
        int s = q.Size();
        if (s != argument) {
          throw TestFailure("Size: expected " + std::to_string(argument) +
                            ", got " + std::to_string(s));
        }
      } break;
    }
  }
};

template <>
struct SerializationTraits<QueueOp> : UserSerTraits<QueueOp, std::string, int> {
};

void QueueTester(const std::vector<QueueOp>& ops) {
  Queue q(0);
  for (auto& op : ops) {
    op.execute(q);
  }
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"ops"};
  return GenericTestMain(args, "circular_queue.cc", "circular_queue.tsv",
                         &QueueTester, DefaultComparator{}, param_names);
}
