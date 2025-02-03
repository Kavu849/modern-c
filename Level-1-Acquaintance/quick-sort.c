// Program implementing the quickSort algorithm using Lomuto's parition
// scheme
#include <stdio.h>

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
  int A[7] = {
    [0] = 5.0,
    [1] = 13.0,
    [2] = 6.0,
    [3] = 9.0,
    [4] = 12.0,
    [5] = 11.0,
    [6] = 8.0,
  };

  printf("Printing the array before the parition: A = [ ");
  for (int i = 0; i <= 6; ++i)
    printf("%d ", A[i]);
  printf("]\n");

  partitionLomuto(A, 0, 6);

  printf("Printing the array after the partition: A = [ ");
  for (int i = 0; i <= 6; ++i)
    printf("%d ", A[i]);
  printf("]\n");

  quickSort(A, 0, 6);

  printf("Printing the arrat after calling quickSort: A = [ ");
  for (int i = 0; i <=6; ++i)
    printf("%d ", A[i]);
  printf("]\n");

  return 0;
}


