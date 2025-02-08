#include <stdio.h>
#include <math.h>

// Program approximating the derivative of a given function at a given point
// using the centered difference scheme for a range of step sizes given as
// powers of 2

void numDer(double (*func)(double), double x0) {
  // Compute the derivative of the function fun at point x0, using the central
  // difference scheme, for varying step sizes h
  double h;
  double res;

  for (int i = 1; i < 15; ++i) {
    h = pow(2.0, -i);
    res = ((*func)(x0 + h) -(*func)(x0 - h)) / (2*h);
    printf("Step size: %f, the value of the approximation at %f: %f \n", h, x0, res);
  }
}

int main() {
  double (*func)(double);
  func = sin;
  double x0 = 1;

  numDer(func, x0);

  return 0;
}
