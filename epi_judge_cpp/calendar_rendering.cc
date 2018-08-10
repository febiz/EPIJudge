#include <algorithm>
#include <queue>
#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
using std::max;
using std::priority_queue;
using std::vector;
using std::sort;

struct Event {
  int start, finish;
};

struct MinHeapFinishCompare {
    bool operator() (const Event& e1, const Event& e2) {
        return e1.finish > e2.finish;
    }
};

int FindMaxSimultaneousEvents(const vector<Event>& A) {
    vector<Event> events(A.begin(), A.end());
    sort(events.begin(), events.end(),
         [](const Event& e1, const Event& e2) {return e1.start < e2.start;});

    // min-heap for event finish time
    priority_queue<Event, vector<Event>, MinHeapFinishCompare> active_events;

    size_t max_concurrent_events = 1;
    for (Event& e : events) {
        while (!active_events.empty() && active_events.top().finish < e.start) {
            // remove finished events
            active_events.pop();
        }
        active_events.push(e);
        max_concurrent_events = max(active_events.size(), max_concurrent_events);
    }
    return max_concurrent_events;
}

template <>
struct SerializationTraits<Event> : UserSerTraits<Event, int, int> {};

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(args, "calendar_rendering.cc",
                         "calendar_rendering.tsv", &FindMaxSimultaneousEvents,
                         DefaultComparator{}, param_names);
}
