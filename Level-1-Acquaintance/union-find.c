#include <stdint.h>
#include <assert.h>
#include <stdio.h>
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

size_t FindCompress(size_t ind, size_t parent[]) {
  size_t root = Find(ind, parent);
  FindReplace(ind, root, parent);
  return root;
}

void Union(size_t ind1, size_t ind2, size_t parent[]) {
  size_t root1 = FindCompress(ind1, parent);
  size_t root2 = Find(ind2, parent);
  if (root1 != root2) {
    FindReplace(ind2, root1, parent);
    parent[root2] = root1;
  }
}

void print_parent_array(size_t n, size_t parent[]) {
    printf("Parent Array (size %zu):\n", n);
    for (size_t i = 0; i < n; ++i) {
        if (parent[i] == SIZE_MAX) {
            printf("[%zu]: ROOT ", i);
        } else {
            printf("[%zu]: %zu ", i, parent[i]);
        }
    }
    printf("\n");
}

void initialize_parent(size_t n, size_t parent[]) {
    for (size_t i = 0; i < n; ++i) {
        parent[i] = SIZE_MAX;
    }
}

int main(int argc, char* argv[argc+1]) {
  int N = 5;
  size_t parent[N];

  // --- Test Case 1: Basic Unions ---
    printf("\n--- Test Case 1: Basic Unions ---\n");
    initialize_parent(N, parent);
    print_parent_array(N, parent);

    Union(0, 1, parent);
    printf("Union(0, 1):\n");
    print_parent_array(N, parent);
    assert(Find(0, parent) == Find(1, parent));

    Union(2, 3, parent);
    printf("Union(2, 3):\n");
    print_parent_array(N, parent);
    assert(Find(2, parent) == Find(3, parent));

    Union(0, 3, parent);
    printf("Union(0, 3):\n");
    print_parent_array(N, parent);
    assert(Find(0, parent) == Find(1, parent));
    assert(Find(0, parent) == Find(2, parent));
    assert(Find(0, parent) == Find(3, parent));

    // --- Test Case 2: Unioning already connected elements ---
    printf("\n--- Test Case 2: Unioning already connected elements ---\n");
    initialize_parent(N, parent);
    Union(0, 1, parent);
    Union(1, 0, parent); // Should have no effect
    printf("Union(0, 1) then Union(1, 0):\n");
    print_parent_array(N, parent);
    assert(Find(0, parent) == Find(1, parent));

    // --- Test Case 3: Path Compression and Replacement ---
    printf("\n--- Test Case 3: Path Compression and Replacement ---\n");
    N = 6;
    initialize_parent(N, parent);
    parent[1] = 0;
    parent[2] = 1;
    parent[3] = 2;
    parent[4] = 3;
    print_parent_array(N, parent);

    FindCompress(4, parent);
    printf("FindCompress(4):\n");
    print_parent_array(N, parent);
    assert(parent[4] == 0);
    assert(parent[3] == 0);
    assert(parent[2] == 0);
    assert(parent[1] == 0);
    assert(Find(4, parent) == 0);

    initialize_parent(N, parent);
    parent[1] = 0;
    Union(1, 5, parent);
    printf("Union(1, 5):\n");
    print_parent_array(N, parent);
    size_t root_of_1 = Find(1, parent);
    size_t root_of_5 = Find(5, parent);
    assert(root_of_1 == root_of_5);
    initialize_parent(N, parent);
    parent[5] = 4;
    Union(0, 5, parent);
    printf("Union(0, 5):\n");
    print_parent_array(N, parent);
    assert(Find(0, parent) == Find(5, parent));

    // --- Test Case 4: Larger set and more operations ---
    printf("\n--- Test Case 4: Larger set and more operations ---\n");
    N = 10;
    initialize_parent(N, parent);
    Union(0, 1, parent);
    Union(2, 3, parent);
    Union(4, 5, parent);
    Union(6, 7, parent);
    Union(8, 9, parent);
    printf("Initial unions:\n");
    print_parent_array(N, parent);

    Union(0, 2, parent);
    printf("Union(0, 2):\n");
    print_parent_array(N, parent);
    assert(Find(0, parent) == Find(3, parent));

    Union(4, 8, parent);
    printf("Union(4, 8):\n");
    print_parent_array(N, parent);
    assert(Find(4, parent) == Find(9, parent));

    Union(0, 4, parent);
    printf("Union(0, 4):\n");
    print_parent_array(N, parent);
    assert(Find(0, parent) == Find(9, parent));

  return 0;
}
