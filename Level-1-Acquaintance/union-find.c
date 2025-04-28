#include <stdint.h>
#include <stdlib.h>

size_t Find(size_t ind, size_t parent[]) {
  while (parent[ind] != SIZE_MAX) {
    ind = parent[ind];
  }
  return ind;
}

void FindReplace(size_t ind, size_t val, size_t parent[]) {
  while (parent[ind] != SIZE_MAX) {
    size_t tmp = parent[ind];
    parent[ind] = val;
    ind = tmp;
  }
}

void FindCompress(size_t ind, size_t parent[]) {
  size_t root = Find(ind, parent);
  FindReplace(ind, root, parent);
}

void Union(size_t ind1, size_t ind2, size_t parent[]) {
  
}

int main(int argc, char* argv[argc+1]) {
  int N = 20; 
  size_t parent[N];

  for (size_t i = 0; i < N; ++i) {
    parent[i] = SIZE_MAX;
  }

  return 0;
}
