#include <stdio.h>
#include <float.h>
#include <math.h>

double dotProd(size_t n, size_t m, double x[], double y[]) {
  if (n != m) {
    printf("Vectors have wrong dimensions! Returning 0.0\n");
    return 0.0;
  }
  double res = 0.0;
  for (size_t i = 0; i < n; ++i)
    res += x[i]*y[i];
  return res;
}

void vecMatMul(size_t n, size_t m, size_t k, double A[n][m], double x[k], double y[n]) {
  if (m != k)
    printf("Matrix and vector dimensions are incompatible!\n");
  else {
    for (size_t i = 0; i < n; ++i)
      y[i] = dotProd(m, k, A[i], x);
  }
}

void printArray(size_t n, size_t m, double A[n][m]) {
  printf("Matrix (%zux%zu):\n", n, m);
  printf("[\n");
  for (size_t i = 0; i < n; ++i) {
    printf(" "); 
    for (size_t j = 0; j < m; ++j) {
      printf("% .2f", A[i][j]);
      if (j < m - 1)
        printf(", ");
    }
    printf("\n");
  }
  printf(" ]\n");
}

void swapRows(size_t n, size_t m, double A[n][m], size_t i, size_t k) {
  for (size_t j = 0; j < m; ++j) {
    double tmp = A[i][j];
    A[i][j] = A[k][j];
    A[k][j] = tmp;
  }
}

void gaussElim(size_t n, size_t m, double A[n][m]) {
  for (size_t j = 0; j < m; ++j) {
    if (j == n) // Check if we are not out of bounds
      break;
    if (fabs(A[j][j]) < DBL_MIN) { // If a diagonal element is zero, look for other candidates
      for (size_t i = j + 1; i < n; ++i) {
        if (fabs(A[i][j]) < DBL_MIN)
          continue;
        // Otherwise, we found a swap element, swap rows j and i
        swapRows(n, m, A, i, j);
        break;
      }
      if (fabs(A[j][j]) < DBL_MIN)
        continue;
    }
    // Now, we have a nonzero pivot at the (j,j) position
    double pivot = A[j][j];
    // Divide the jth row by the pivot
    for (size_t k = j; k < m; ++k)
      A[j][k] /= pivot;
    
    // Adjust all the rows below the jth row
    for (size_t i = j + 1; i < n; ++i) {
      pivot = A[i][j];
      if (fabs(pivot) < DBL_MIN)
        continue;
      A[i][j] = 0.0;
      for (size_t k = j + 1; k < m; ++k)
        A[i][k] -= pivot * A[j][k];
    }
  }
}

int main(int argc, char* argv[argc+1]) {
  size_t m = 10;
  size_t k = 10;
  size_t n = 7;

  double x[] = { 1.3, 2.4, 3.2, 6.2, 6.3, 3.2, 9.6, 4.3, 3.2, 1.77,};
  double y[] = { 1.2, 2.1, 1.1, 6.7, 8.3, 5.5, 6.5, 9.0, 7.6, 3.44,};
  double A[][10] = {
        {5.3, 1.8, 9.2, 0.7, 4.0, 7.5, 2.1, 8.6, 3.9, 6.4,},
        {1.0, 7.2, 3.5, 9.8, 2.3, 5.7, 0.6, 4.9, 8.1, 6.0,},
        {4.5, 0.9, 8.0, 1.2, 7.8, 3.4, 9.1, 5.6, 2.7, 0.3,},
        {8.7, 2.0, 6.5, 0.4, 9.9, 1.1, 7.3, 3.8, 5.2, 4.6,},
        {3.1, 9.5, 0.0, 6.2, 1.7, 8.4, 4.8, 7.1, 2.6, 5.9,},
        {7.4, 4.1, 2.8, 5.5, 0.2, 9.6, 1.9, 6.7, 3.0, 8.3,},
        {0.5, 6.8, 1.3, 3.7, 8.9, 2.4, 5.0, 9.3, 4.2, 7.6,},
    };
  
  double z = dotProd(m, k, x, y);
  double w[n];
  vecMatMul(n, m, k, A, x, w);

  printf("Original array:\n");
  printArray(n, m, A);
  
  printf("After Gaussian elimination:\n");
  gaussElim(n, m, A);
  printArray(n, m, A); 

  return 0;
}
