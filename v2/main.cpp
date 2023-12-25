#include <iostream>
#include <vector>
#include <stack>
#include <chrono>
#include <omp.h>

class Graph {
  int numVertices;
  std::vector<std::vector<int>> adjLists;
  std::vector<bool> visited;

public:
  Graph(int vertices) : numVertices(vertices), adjLists(vertices), visited(vertices, false) {}

  void addEdge(int src, int dest) {
    adjLists[src].push_back(dest);
  }

  void DFS(int startVertex) {
    std::stack<int> stack;
    stack.push(startVertex);

    while (!stack.empty()) {
      int currentVertex = stack.top();
      stack.pop();

      if (!visited[currentVertex]) {
        visited[currentVertex] = true;
        std::cout << currentVertex << " ";

        for (auto i = adjLists[currentVertex].begin(); i != adjLists[currentVertex].end(); ++i) {
          if (!visited[*i]) {
            stack.push(*i);
          }
        }
      }
    }
  }

  void parallelDFS() {
#pragma omp parallel for
    for (int startVertex = 0; startVertex < numVertices; startVertex++) {
      if (!visited[startVertex]) {
        DFS(startVertex);
      }
    }
  }
};

int main() {
  int numVertices = 5000;
  Graph g(numVertices);

  for (int i = 0; i < numVertices; ++i) {
    for (int j = i + 1; j < i + 5; ++j) {
      g.addEdge(i, j % numVertices);
    }
  }

  auto start = std::chrono::high_resolution_clock::now();
  g.parallelDFS();
  auto end = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double> diff = end - start;
  std::cout << "\nTime taken by DFS: " << diff.count() << " seconds" << std::endl;

  return 0;
}
