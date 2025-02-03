// Program implementing a recursive merge-sort algorithm on an array

#include <stdio.h>

void mergeArrays(double A[], int l, int m, int r) {
  // At this point, A is sorted from l to m, and from m+1 to r. We need to
  // merge two two arrays, so that A is sorted from l to r

  // We will copy the sorted arrays to merge into two temporary arrays
  int i, j, k;

  int n1 = m - l + 1;
  int n2 = r - m;

  double L[n1], R[n2];

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
}

void mergeSort(double A[], int l, int r) {
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

int main(int argc, char* argv[argc + 1]) {
  double A[5] = {
    [0] = 9.0,
    [1] = 2.9,
    [4] = 3.E+25,
    [3] = .00007,
  };
  mergeSort(A, 0, 4);

  printf("The sorted array is:\n A = [%f", A[0]);
  for (int i = 1; i < 5; ++i) {
    printf(", %f", A[i]);
  }
  printf("]\n");
  return 0;
}
