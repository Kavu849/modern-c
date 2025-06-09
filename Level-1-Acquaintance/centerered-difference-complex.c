#include <stdio.h>
#include <math.h>
#include <complex.h>

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

void numDerCmplx(double complex (*func)(double complex x), double complex x0) {
  // Compute the derivative of the complex function passed as a function
  // pointer. We are using the central difference scheme.
  double complex h, res;
  
  // Derivative of the real part
  for (int i = 1; i < 15; ++i) {
    h = cpow(2.0, -i);
    res = ((*func)(x0 + h*CMPLX(0.0, 1.0)) - (*func)(x0 - h*CMPLX(0.0, 1.0))) / (2*h*CMPLX(0.0, 1.0));
    printf("Step size: h = (%f, %f), the value of the approximation at " 
        "x0 = (%f, %f) is (%f, %f)\n", creal(h), cimag(h), creal(x0), cimag(x0),
        creal(res), cimag(res));
  }
}

int main() {
  double (*func)(double); 
  double complex (*cfunc)(double complex); 

  func = sin;
  cfunc = csin;

  double x0 = 1;
  double complex cx0 = CMPLX(1.0, 1.0);

  numDer(func, x0);
  numDerCmplx(cfunc, cx0);

  return 0;
}
