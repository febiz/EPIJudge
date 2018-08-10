#include <istream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
using std::vector;
using std::unordered_map;
using std::unordered_set;

typedef enum { kWhite, kBlack } Color;
struct Coordinate {
  bool operator==(const Coordinate& that) const {
    return x == that.x && y == that.y;
  }

  int x, y;
};

struct CoordinateHash {
public:
  std::size_t operator()(const Coordinate &c) const
  {
    return std::hash<int>()(c.x) ^ std::hash<int>()(c.y);
  }
};

bool helper(unordered_map<Coordinate, vector<Coordinate>, CoordinateHash>& graph,
            unordered_set<Coordinate, CoordinateHash>& visited,
            vector<Coordinate>& result, Coordinate s, Coordinate e) {
    if (s == e) {
        return true;
    }
    if (visited.count(s) > 0) {
        return false;
    }
    visited.insert(s);
    for(auto it = graph[s].begin(); it != graph[s].end(); ++it) {
        result.push_back(*it);
        if (helper(graph, visited, result, *it, e)) {
            return true;
        }
        result.pop_back();
    }
    return false;
}

vector<Coordinate> SearchMaze(vector<vector<Color>> maze, const Coordinate& s,
                              const Coordinate& e) {
  unordered_map<Coordinate, vector<Coordinate>, CoordinateHash> graph;
  for (int i = 0; i < maze.size(); ++i) {
      for (int j = 0; j < maze[i].size(); ++j) {
          if (maze[i][j] == kWhite) {
              Coordinate curr = {i,j};
              if (i - 1 >= 0 && maze[i-1][j] == kWhite) {
                  graph[curr].push_back(Coordinate{i-1, j});
              }
              if (i + 1 < maze.size() && maze[i+1][j] == kWhite) {
                  graph[curr].push_back(Coordinate{i+1, j});
              }
              if (j - 1 >= 0 && maze[i][j-1] == kWhite) {
                  graph[curr].push_back(Coordinate{i, j-1});
              }
              if (j + 1 < maze[i].size() && maze[i][j+1] == kWhite) {
                  graph[curr].push_back(Coordinate{i, j+1});
              }

          }
      }
  }
  unordered_set<Coordinate, CoordinateHash> visited;
  vector<Coordinate> result = {s};
  bool found = helper(graph, visited, result, s, e);
  return found ? result : vector<Coordinate>{};
}

template <>
struct SerializationTraits<Color> : SerializationTraits<int> {
  using serialization_type = Color;

  static serialization_type Parse(const std::string& str) {
    return static_cast<serialization_type>(
        SerializationTraits<int>::Parse(str));
  }

  static serialization_type JsonParse(const json_parser::Json& json_object) {
    return static_cast<serialization_type>(
        SerializationTraits<int>::JsonParse(json_object));
  }
};

template <>
struct SerializationTraits<Coordinate> : UserSerTraits<Coordinate, int, int> {
  static std::vector<std::string> GetMetricNames(const std::string& arg_name) {
    return {};
  }

  static std::vector<int> GetMetrics(const Coordinate& x) { return {}; }
};

bool PathElementIsFeasible(const vector<vector<Color>>& maze,
                           const Coordinate& prev, const Coordinate& cur) {
  if (!(0 <= cur.x && cur.x < maze.size() && 0 <= cur.y &&
        cur.y < maze[cur.x].size() && maze[cur.x][cur.y] == kWhite)) {
    return false;
  }
  return cur == Coordinate{prev.x + 1, prev.y} ||
         cur == Coordinate{prev.x - 1, prev.y} ||
         cur == Coordinate{prev.x, prev.y + 1} ||
         cur == Coordinate{prev.x, prev.y - 1};
}

bool SearchMazeWrapper(TimedExecutor& executor,
                       const vector<vector<Color>>& maze, const Coordinate& s,
                       const Coordinate& e) {
  vector<vector<Color>> copy = maze;

  auto path = executor.Run([&] { return SearchMaze(copy, s, e); });

  if (path.empty()) {
    return s == e;
  }

  if (!(path.front() == s) || !(path.back() == e)) {
    throw TestFailure("Path doesn't lay between start and end points");
  }

  for (size_t i = 1; i < path.size(); i++) {
    if (!PathElementIsFeasible(maze, path[i - 1], path[i])) {
      throw TestFailure("Path contains invalid segments");
    }
  }

  return true;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "maze", "s", "e"};
  return GenericTestMain(args, "search_maze.cc", "search_maze.tsv",
                         &SearchMazeWrapper, DefaultComparator{}, param_names);
}
