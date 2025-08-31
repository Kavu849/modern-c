#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
      //A[k] = L[i];
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

void mergeSort(void* A, size_t size, compareFcn* cmp, size_t l,
               size_t r) {
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

int main() {
  signed A[7] = {
    [0] = 5,
    [1] = 13,
    [2] = 6,
    [3] = 9,
    [4] = 12,
    [5] = 11,
    [6] = 8,
  };

  mergeSortSigned(7, A);
  printf("Printing the array after calling quickSort: A = [ ");
  for (int i = 0; i <=6; ++i)
    printf("%d ", A[i]);
  printf("]\n");

  return 0;
}


