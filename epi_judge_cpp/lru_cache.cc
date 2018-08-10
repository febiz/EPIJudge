#include <deque>
#include <set>
#include <vector>
#include <unordered_map>
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include "test_framework/test_failure.h"
using std::map;
using std::pair;
using std::unordered_map;

struct PrioAndPrice {
    long priority;
    int price;
};

class LruCache {
 public:
  LruCache(size_t capacity) : capacity_(capacity), cnt_(0) {}
  int Lookup(int isbn) {
      int price = -1;
      auto search = cache_.find(isbn);
      if (search != cache_.end()) {
          price = search->second.price;
          UpdatePriority(isbn, search->second.priority);
      }

//      Debug("Lookup " + std::to_string(isbn));
      return price;
  }

  void Insert(int isbn, int price) {
      auto search = cache_.find(isbn);
      if (search != cache_.end()) {
          UpdatePriority(isbn, search->second.priority);
      } else {
          if (cache_.size() == capacity_) {
              // find entry with lowerst priority and erase from both maps
              auto prio_last_it = std::prev(prio_.end());
              cache_.erase(prio_last_it->second);
              prio_.erase(prio_last_it);
          }
          cache_[isbn] = {cnt_, price};
          prio_[cnt_++] = isbn;
      }

//      Debug("Insert (" + std::to_string(isbn) + ", " + std::to_string(price) + ")");
  }

  bool Erase(int isbn) {
      auto search = cache_.find(isbn);
      if (search != cache_.end()) {
          prio_.erase(search->second.priority);
          cache_.erase(search);

//          Debug("Erase " + std::to_string(isbn));
          return true;
      } else {

//          Debug("Erase " + std::to_string(isbn));
          return false;
      }
  }

private:
  void UpdatePriority(int isbn, int priority) {
      // NOTE: this takes O(log(capacity))
      // better: store pointer to priority list directly in hash map
      prio_.erase(priority);
      cache_[isbn].priority = cnt_;
      prio_[cnt_++] = isbn;
  }

  void Debug(std::string s) {
      std::cout << std::endl << s << std::endl;
      for (auto it : prio_) {
          std::cout << it.second << " -> (" << cache_[it.second].priority << ", " << cache_[it.second].price << ")" << std::endl;
      }
      std::cout << "----------" << std::endl;
  }

private:
  unordered_map<int, PrioAndPrice> cache_;  // isbn -> (priority, price)
  map<long, int, std::greater<>> prio_;     // priority -> isbn
  size_t capacity_;
  long cnt_;
};

struct Op {
  std::string code;
  int arg1;
  int arg2;
};

template <>
struct SerializationTraits<Op> : UserSerTraits<Op, std::string, int, int> {};

void RunTest(const std::vector<Op>& commands) {
  if (commands.empty() || commands[0].code != "LruCache") {
    throw std::runtime_error("Expected LruCache as first command");
  }
  LruCache cache(commands[0].arg1);

  for (int i = 1; i < commands.size(); i++) {
    auto& cmd = commands[i];
    if (cmd.code == "lookup") {
      int result = cache.Lookup(cmd.arg1);
      if (result != cmd.arg2) {
        throw TestFailure("Lookup: expected " + std::to_string(cmd.arg2) +
                          ", got " + std::to_string(result));
      }
    } else if (cmd.code == "insert") {
      cache.Insert(cmd.arg1, cmd.arg2);
    } else if (cmd.code == "erase") {
      bool result = cache.Erase(cmd.arg1);
      if (result != cmd.arg2) {
        throw TestFailure("Erase: expected " + std::to_string(cmd.arg2) +
                          ", got " + std::to_string(result));
      }
    } else {
      throw std::runtime_error("Unexpected command " + cmd.code);
    }
  }
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"commands"};
  return GenericTestMain(args, "lru_cache.cc", "lru_cache.tsv", &RunTest,
                         DefaultComparator{}, param_names);
}
