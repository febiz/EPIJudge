#include <algorithm>
#include <queue>
#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include "test_framework/timed_executor.h"
using std::priority_queue;
using std::vector;
using std::sort;

struct Interval {
  struct Endpoint {
    bool is_closed;
    int val;
  };

  Endpoint left, right;
};

struct Comp {
    bool operator() (const Interval& a, const Interval& b) {
        if (a.right.val == b.right.val) {
            return a.right.is_closed;
        } else {
            return a.right.val > b.right.val;
        }
    }
};

vector<Interval> UnionOfIntervals(vector<Interval> intervals) {
    sort(intervals.begin(), intervals.end(),
         [](const Interval& a, const Interval& b) {
        if (a.left.val == b.left.val) {
            return a.left.is_closed;
        } else {
            return a.left.val < b.left.val;
        }
    });

    // NOTE: A bit of an overkill, could simply check if intersects
    // with previous interval and update accordingly instead of using
    // a queue of active intervals

    priority_queue<Interval, vector<Interval>, Comp> active_intervals;

    vector<Interval> result;

    Interval start = intervals.front();
    for (Interval& i : intervals) {
        Interval last = {{true, 0}, {true, -1}};
        while (!active_intervals.empty()) {
            Interval curr = active_intervals.top();
            // Intervals do not overlap if right.val < left.val
            // or if val is equal and both start and finish are open
            if (curr.right.val < i.left.val ||
                    (curr.right.val == i.left.val &&
                     !curr.right.is_closed && !i.left.is_closed)) {
                last = active_intervals.top();
                active_intervals.pop();
            } else {
                break;
            }
        }
        // no active intervals, add to result if last is valid
        if (active_intervals.empty() && last.left.val <= last.right.val) {
            result.push_back({{start.left.is_closed, start.left.val},
                              {last.right.is_closed, last.right.val}});
            start = i;
        }
        active_intervals.push(i);
    }

    // add last if there are still active intervals
    if (!active_intervals.empty()) {
        Interval last;
        while (!active_intervals.empty()) {
            last = active_intervals.top();
            active_intervals.pop();
        }
        result.push_back({{start.left.is_closed, start.left.val},
                          {last.right.is_closed, last.right.val}});
    }

    return result;
}
struct FlatInterval {
  int left_val;
  bool left_is_closed;
  int right_val;
  bool right_is_closed;

  FlatInterval(int left_val, bool left_is_closed, int right_val,
               bool right_is_closed)
      : left_val(left_val),
        left_is_closed(left_is_closed),
        right_val(right_val),
        right_is_closed(right_is_closed) {}

  explicit FlatInterval(Interval in)
      : left_val(in.left.val),
        left_is_closed(in.left.is_closed),
        right_val(in.right.val),
        right_is_closed(in.right.is_closed) {}

  operator Interval() const {
    return {{left_is_closed, left_val}, {right_is_closed, right_val}};
  }

  bool operator==(const FlatInterval& rhs) const {
    return std::tie(left_val, left_is_closed, right_val, right_is_closed) ==
           std::tie(rhs.left_val, rhs.left_is_closed, rhs.right_val,
                    rhs.right_is_closed);
  }
};

template <>
struct SerializationTraits<FlatInterval>
    : UserSerTraits<FlatInterval, int, bool, int, bool> {};

std::ostream& operator<<(std::ostream& out, const FlatInterval& i) {
  return out << (i.left_is_closed ? '<' : '(') << i.left_val << ", "
             << i.right_val << (i.right_is_closed ? '>' : ')');
}

std::vector<FlatInterval> UnionOfIntervalsWrapper(
    TimedExecutor& executor, const std::vector<FlatInterval>& intervals) {
  std::vector<Interval> casted;
  for (const FlatInterval& i : intervals) {
    casted.push_back(static_cast<Interval>(i));
  }

  std::vector<Interval> result =
      executor.Run([&] { return UnionOfIntervals(casted); });

  return {begin(result), end(result)};
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "intervals"};
  return GenericTestMain(args, "intervals_union.cc", "intervals_union.tsv",
                         &UnionOfIntervalsWrapper, DefaultComparator{},
                         param_names);
}
