#include <complex.h>
#include <math.h>
#include <stdio.h>

#define fabs(X)         \
_Generic((X),           \
  float: fabsf,         \
  long double: fabsl,   \
  double complex: cabs, \
  default: fabs) (X)

int main(int argc, char* argv[argc+1]) {
  double res = fabs(-1);
  double complex z = CMPLX(-1.0, 1.0);
  double resc = fabs(z);

  printf("Res: %g\n", res);
  printf("Resc: %g\n", resc);
  return 0;
}
