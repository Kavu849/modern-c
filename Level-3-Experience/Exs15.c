#include <stdio.h>
#include <stddef.h>

#define SUM_DOUBLE(...) sumIt(sizeof((double[]){ __VA_ARGS__ }) / sizeof(double), (double[]) { __VA_ARGS__ })

static inline double sumIt(size_t n, double* a) {
  double ret = 0.0;
  for (size_t i = 0; i < n; ++i) {
    ret += a[i];
  }
  return ret;
}

int main(void) {
  double ret = SUM_DOUBLE( 1.0, 2.0, 1.0, -1.0, 0.0 );
  printf("Sum: %f\n", ret);
  return 0;
}
