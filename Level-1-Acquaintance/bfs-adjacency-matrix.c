#include <stddef.h>
#include <stdio.h>

void bfsFindConnected(size_t n, size_t startNode, bool A[n][n], bool visited[n]) {
  if (startNode >= n) {
    printf("Wrong size/starting node supplied! Exiting\n");
    return;
  }
  size_t queue[n];
  unsigned front = -1, rear = -1;
  size_t path[n];
  
  // Enqueue startNode and mark it as visited
  queue[++rear] = startNode;
  visited[startNode] = true;

  while (front != rear) {
    // Dequeue the node from the beginning of the queue and add it to the path
    size_t currentNode = queue[++front];
    path[front] = currentNode;

    for (size_t i = 0; i < n; ++i) {
      // Enqueue all the nodes that are adjacent and not visited yet
      if (A[i][currentNode] && !visited[i]) {
        visited[i] = true;
        queue[++rear] = i;
      }
    }
  }
  printf("Path starting with node %ld: ", startNode);
  for (size_t i = 0; i <= front; ++i)
    printf("%ld, ", path[i]);
  printf("\n");
}

void findAllConnected(size_t n, bool A[n][n]) {
  bool visited[n];
  for (size_t i = 0; i < n; ++i)
    visited[i] = false;

  for (size_t i = 0; i < n; ++i) {
    if (!visited[i])
      bfsFindConnected(n, i, A, visited);
  }
}

int main(int argc, char* argv[argc+1]) {
  constexpr size_t n = 7; // Number of nodes in the graph
  bool A[n][n] = { {0, 1, 0, 0, 0, 0, 0},
                   {1, 0, 0, 0, 1, 1, 0},
                   {0, 0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0, 1},
                   {0, 1, 0, 0, 0, 1, 0},
                   {0, 1, 0, 0, 1, 0, 0},
                   {0, 0, 0, 1, 0, 0, 0}};
  findAllConnected(n, A);
}
