#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Generic comparison function. The convention is that it returns a negative
// number if a < b, a positive number if a > b, and 0 if a == b
typedef int compareFcn(void const* a, void const* b);

// ----------------------Compare functions for various types-------------------
int compareSigned(void const* a, void const* b) {
  signed const* A = a;
  signed const* B = b;
  if (*A < *B)
    return -1;
  else if (*A > *B)
    return 1;
  else
    return 0;
}

// -------------------------Merge sort-related functions-----------------------
void mergeArrays(void* A, size_t size, compareFcn* cmp, size_t l,
                 size_t m, size_t r) {
  // At this point, A is sorted from l to m, and from m+1 to r. We need to
  // merge two two arrays, so that A is sorted from l to r

  // We will copy the sorted arrays to merge into two temporary arrays
  size_t n1 = m - l + 1;
  size_t n2 = r - m;

  void* L = malloc(n1 * size);
  void* R = malloc(n2 * size);

  memcpy((char*)L, (char*)A + l * size, n1 * size);
  memcpy((char*)R, (char*)A + (m + 1) * size, n2 * size);

  size_t i, j, k;
  i = 0;
  j = 0;
  k = l;

  // Now, update the values of A so that they are sorted in the range [l, r]
  while (i < n1 &&  j < n2) {
    if (cmp((char*)L + i * size, (char*)R + j * size) <= 0) {
      memcpy((char*)A + k * size, (char*)L + i * size, size);
      ++i;
    }
    else {
      memcpy((char*)A + k * size, (char*)R + j * size, size);
      ++j;
    }
    ++k;
  }

  if (i < n1)
    memcpy((char*)A + k * size, (char*)L + i * size, (n1 - i) * size);

  if (j < n2)
    memcpy((char*)A + k * size, (char*)R + j * size, (n2 - j) * size);

  free(L);
  free(R);
}

void mergeSort(void* A, size_t size, compareFcn* cmp, size_t l, size_t r) {
  // If the size is one, return
  if (r <= l)
    return;

  // Otherwise, split the array in half and continue
  size_t m = (l + r) / 2;

  mergeSort(A, size, cmp, l, m);
  mergeSort(A, size, cmp, m + 1, r);

  // Now, merge the arrays
  mergeArrays(A, size, cmp, l, m, r);
}

void mergeSortSigned(size_t n, signed A[n]) {
  mergeSort(A, sizeof A[0], compareSigned, 0, n - 1);
}

// -------------------------Quicksort-related functions------------------------
int partitionLomuto(void* A, size_t size, compareFcn* cmp, size_t l,
                    size_t r) {
  size_t i = l;
  char* pivot = malloc(size);
  memcpy(pivot, (char*)A + r * size, size);
  char* temp = malloc(size);

  for (size_t j = i; j < r; ++j) {
    if (cmp((char*)A + j * size, pivot) <= 0) {
      // perform the swap
      memcpy(temp, (char*)A + i * size, size);
      memcpy((char*)A + i * size, (char*)A + j * size, size);
      memcpy((char*)A + j * size, temp, size);
      ++i;
    }
  }

  // now, all the elements with indices < i, are smaller than the pivot
  // place the pivot at the position i
  memcpy((char*)A + r * size, (char*)A + i * size, size);
  memcpy((char*)A + i * size, pivot, size);
  free(temp);
  free(pivot);

  return i;
}

void quickSort(void* A, size_t size, compareFcn* cmp, size_t l, size_t r) {
  if (l < r) {
    size_t m = partitionLomuto(A, size, cmp, l, r);
    quickSort(A, size, cmp, l, m - 1);
    quickSort(A, size, cmp, m + 1, r);
  }
}

void quickSortSigned(size_t n, signed A[n]) {
  quickSort(A, sizeof A[0], compareSigned, 0, n - 1);
}

// qsort helper function
void qsortSigned(size_t n, signed A[n]) {
  qsort(A, n, sizeof A[0], compareSigned);
}

int main() {
  constexpr size_t n = 7;

  signed A[n] = {
    [0] = 5,
    [1] = 13,
    [2] = 6,
    [3] = 9,
    [4] = 12,
    [5] = 11,
    [6] = 8,
  };

  signed B[n];
  memcpy(B, A, n * sizeof(signed));

  mergeSortSigned(n, A);
  printf("Printing the array after calling merge sort: A = [ ");
  for (int i = 0; i < n; ++i)
    printf("%d ", A[i]);
  printf("]\n");

  quickSortSigned(n, B);
  printf("Printing the array after calling quick sort: A = [ ");
  for (int i = 0; i < n; ++i)
    printf("%d ", B[i]);
  printf("]\n");

  // Sorting for larger array sizes
  srand(time(0));
  size_t m = 100000000;
  printf("Size of array: %zu\n", m);

  // Allocate memory for the arrays
  signed* arr1 = malloc(m * sizeof(signed));
  signed* arr2 = malloc(m * sizeof(signed));
  signed* arr3 = malloc(m * sizeof(signed));

  // Populate the array with numbers from range [0, n], and copy the elements
  // to the second array
  for (size_t i = 0; i < m; ++i)
    arr1[i] = rand() % (m + 1);
  memcpy(arr2, arr1, m * sizeof(signed));
  memcpy(arr3, arr1, m * sizeof(signed));

  clock_t start = clock();
  mergeSortSigned(m, arr1);
  clock_t end = clock();
  double diff = (double)(end - start) / CLOCKS_PER_SEC;
  printf("Time for sorting using mergeSortSigned: %.9f\n", diff);

  start = clock();
  quickSortSigned(m, arr2);
  end = clock();
  diff = (double)(end - start) / CLOCKS_PER_SEC;
  printf("Time for sorting using quickSortSigned: %.9f\n", diff);

  start = clock();
  qsortSigned(m, arr3);
  end = clock();
  diff = (double)(end - start) / CLOCKS_PER_SEC;
  printf("Time for sorting using qsortSigned: %.9f\n", diff);

  free(arr1);
  free(arr2);
  free(arr3);
  return 0;
}
