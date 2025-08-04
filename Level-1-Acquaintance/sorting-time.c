// Challenge 10: comparison of different sorting algorithm
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Functions for merge sort
void mergeArrays(int A[], int l, int m, int r) {
  // At this point, A is sorted from l to m, and from m+1 to r. We need to
  // merge two two arrays, so that A is sorted from l to r

  // We will copy the sorted arrays to merge into two temporary arrays
  int i, j, k;

  int n1 = m - l + 1;
  int n2 = r - m;

  // Allocate memory for the temporary arrays on the heap
  int* L = (int*)malloc(n1 * sizeof(int));
  int* R = (int*)malloc(n2 * sizeof(int));
  if (L == nullptr || R == nullptr) {
    perror("Failed to allocate memory for array");
    exit(EXIT_FAILURE);
  }

  for (i = 0; i < n1; ++i)
    L[i] = A[l + i];
  for (j = 0; j < n2; ++j)
    R[j] = A[m + 1 + j];

  i = 0;
  j = 0;
  k = l;

  // Now, update the values of A so that they are sorted in the range [l, r]
  while (i < n1 &&  j < n2) {
    if (L[i] <= R[j]) {
      A[k] = L[i];
      ++i;
    }
    else {
      A[k] = R[j];
      ++j;
    }
    ++k;
  }

  while (i < n1) {
    A[k] = L[i];
    ++i;
    ++k;
  }

  while (j < n2) {
    A[k] = R[j];
    ++j;
    ++k;
  }

  free(L);
  free(R);
}

void mergeSort(int A[], int l, int r) {
  // If the size is one, return
  if (r <= l)
    return;

  // Otherwise, split the array in half and continue
  int  m = (l + r) / 2;

  mergeSort(A, l, m);
  mergeSort(A, m + 1, r);

  // Now, merge the arrays
  mergeArrays(A, l, m, r);
}
// Functions for the quicksort implementation
int partitionLomuto(int A[], int l, int r) {

  int pivot = A[r];

  int i = l;
  int temp;

  for (int j = i; j < r; ++j) {
    if (A[j] < pivot) {
      // perform the swap
      temp = A[i];
      A[i] = A[j];
      A[j] = temp;
      ++i;
    }
  }

  // now, all the elements with indices < i, are smaller than the pivot
  // place the pivot at the position i
  A[r] = A[i];
  A[i] = pivot;
  return i;
}

void quickSort(int A[], int l, int r) {
  if (l < r) {
    int m = partitionLomuto(A, l, r);
    quickSort(A, l, m - 1);
    quickSort(A, m + 1, r);
  }
}

int main(int argc, char* argv[argc+1]) {

  srand(time(0));

  size_t testSizes[] = {
    1000000,
    5000000,
    9000000,
    10000000,
    15000000,
    20000000
  }; 
  size_t numEl = sizeof(testSizes) / sizeof(testSizes[0]);
  
  double times[2][numEl];

  for (size_t i = 0; i < numEl; ++i) {
    size_t currSize = testSizes[i];
    printf("Size of array: %zu\n", currSize);

    // Allocate memory for the arrays
    int* arr1 = (int*)malloc(currSize * sizeof(int));
    int* arr2 = (int*)malloc(currSize * sizeof(int));
    if (arr1 == nullptr || arr2 == nullptr) {
      perror("Failed to allocate memory for array");
      exit(EXIT_FAILURE);
    }

    // Populate the array with random numbers from range [0, currSize], and
    // the copy it to the second array
    for (size_t j = 0; j < currSize; ++j)
      arr1[j] = rand() % (currSize + 1);
    memcpy(arr2, arr1, currSize * sizeof(int));
    
    // Measure times of the sorting algorithms
    time_t start = time(0);
    mergeSort(arr1, 0, currSize - 1);
    time_t end = time(0);
    double diff = difftime(end, start);
    times[0][i] = diff;

    start = time(0);
    quickSort(arr2, 0, currSize - 1);
    end = time(0);
    diff = difftime(end, start);
    times[1][i] = diff;

    // Release the memory;
    free(arr1);
    free(arr2);
  }

  printf("Times of algorithms:\n");
  for (size_t i = 0; i < numEl; ++i) {
    printf("  Size: %zu, mergeSort time: %f, quickSort time: %f\n",
           testSizes[i], times[0][i], times[1][i]);
  }

  return 0;
}
