#include <stddef.h>

void bfsFindConnected(size_t n, size_t startNode, bool A[n][n], size_t visited[n], size_t queue[n]) {
  if (startNode >= n) {
    printf("Wrong size/starting node supplied! Exiting\n");
    return;
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
  size_t visited[n];
  size_t queue[n];
}
