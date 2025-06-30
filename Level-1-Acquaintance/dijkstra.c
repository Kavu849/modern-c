#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

void findShortestPaths(size_t n, size_t A[n][n], size_t startNode, size_t d[n], int p[n]) {
  // S - set of visited nodes
  // Q - set of not visited nodes
  // Create the set QS, where QS[i]==true iff the node 'i' is in S (so not in Q)
  bool QS[n];
  for (size_t i = 0; i < n; ++i)
    QS[i] = false;
  size_t j, k;

  // Iterate n times. In every iteration we move one node from Q to S
  for (size_t i = 0; i < n; ++i) {
    // Look for the node  with the smallest cost of reach d which is also
    // in set Q
    for (j = 0; QS[j]; ++j); // Find the first node in Q
    for (k = j; k < n; ++k)
      if (!QS[k] && (d[k] < d[j]))
        j = k;

    // Move node 'k' from Q to S
    QS[j] = true;

    // Iterate over all neighbors of 'k' and check if it's better to reach the
    // neighbor through node 'k'
    for (size_t m = 0; m < n; ++m) {
      if (A[j][m] == SIZE_MAX)
        continue;
      
      if (!QS[m] && d[m] > d[j] + A[j][m]) {
        d[m] = d[j] + A[j][m];
        p[m] = j;
      }
    }
  }
}

void findShortestDistance(size_t n, size_t A[n][n], size_t startNode, size_t endNode) {
  if (startNode >= n || endNode >= n) {
    printf("Wrong nodes supplied!\n");
    return;
  }

  size_t d[n];
  int p[n];
  for (size_t i = 0; i < n; ++i) {
    d[i] = SIZE_MAX;
    p[i] = -1;
  }
  d[startNode] = 0;
  
  findShortestPaths(n, A, startNode, d, p);

  printf("The shortest path between %zu and %zu is equal to %zu\n", startNode,
      endNode, d[endNode]);
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
  size_t startNode = 0;
  size_t endNode = 4;

  findShortestDistance(n, A, startNode, endNode);
  return 0;
}
