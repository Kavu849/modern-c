#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

size_t findShortestPath(size_t n, size_t A[n][n], size_t startNode, size_t endNode) {
  bool visited[n];
  for (size_t i = 0; i < n; ++i)
    visited[i] = false;
  
  size_t distance[n];
  for (size_t i = 0; i < n; ++i)
    distance[i] = SIZE_MAX;

  size_t queue[n];
  signed front = -1, rear = -1;
  queue[++rear] = startNode;

  while (front != read) {
    size_t currentNode = queue[++front];
    
  }
}

int main(int argc, char* argv[argc+1]) {
  constexpr size_t n = 7;
  size_t A[n][n] = {
    {SIZE_MAX, 2, 3, SIZE_MAX, SIZE_MAX, SIZE_MAX, SIZE_MAX},
    {2, SIZE_MAX, SIZE_MAX, 3, SIZE_MAX, SIZE_MAX, 5},
    {3, SIZE_MAX, SIZE_MAX, SIZE_MAX, SIZE_MAX, SIZE_MAX, SIZE_MAX},
    {SIZE_MAX, 3, SIZE_MAX, SIZE_MAX, 4, 1, SIZE_MAX},
    {SIZE_MAX, SIZE_MAX, SIZE_MAX, 4, SIZE_MAX, 2, SIZE_MAX},
    {SIZE_MAX, SIZE_MAX, SIZE_MAX, 1, 2, SIZE_MAX, SIZE_MAX},
    {SIZE_MAX, 5, SIZE_MAX, SIZE_MAX, SIZE_MAX, SIZE_MAX, SIZE_MAX}
  };
}
