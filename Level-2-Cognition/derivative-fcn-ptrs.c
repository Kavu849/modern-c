#include <complex.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Polynomial Polynomial;
struct Polynomial {
  size_t deg; // degree of the polynomial
  double* coeff; // array of coefficients, must be of size deg + 1
};

static Polynomial* globalPolynomialPtr = nullptr;

double centeredDiffReal(double (*f)(double), double x0) {
  if (!f)
    perror("Wrong function pointer!");
  double h = sqrt(DBL_EPSILON) * (fabs(x0) + 1.0);
  return ((*f)(x0 + h) - (*f)(x0 - h)) / (2 * h);
}

void centeredDiffComplex(double complex (*f)(double complex x), double complex x0) {
  // Compute the derivative of the complex function passed as a function
  // pointer. We are using the central difference scheme.
  double complex h, res;
  
  for (int i = 1; i < 15; ++i) {
    h = cpow(2.0, -i);
    res = ((*f)(x0 + h*CMPLX(0.0, 1.0)) - (*f)(x0 - h*CMPLX(0.0, 1.0))) / (2*h*CMPLX(0.0, 1.0));
    printf("Step size: h = (%f, %f), the value of the approximation at " 
        "x0 = (%f, %f) is (%f, %f)\n", creal(h), cimag(h), creal(x0), cimag(x0),
        creal(res), cimag(res));
  }
}

// Root-finding algorithm implementing Newton's method, where x0 is the
// starting point of the iterations
double findZero(double (*f)(double), double x0) {
  if (fabs(f(x0)) < 1e-14)
    return x0;

  const double tol = 1e-10; // tolerance for the iterations
  double err = tol + 1;
  size_t i = 0;
  const size_t it_max = 100;
  double x1 = x0;

  while (err > tol && i < it_max) {
    double df = centeredDiffReal(f, x0);
    if (fabs(df) < 1e-14) {
      fprintf(stderr, "Derivative too small at x = %f\n", x0);
      break;
    }

    x1 = x0 - (*f)(x0) / df;
    err = fabs(x1 - x0);
    x0 = x1;
    ++i;
  }

  if (i == it_max) {
    printf("Maximum number of iterations reached! Last value found: %f\n", x1);
    return x1;
  }
  
  return x1;
}

// Evaluate polynomial p at x
double evalPoly(Polynomial* p, double x) {
  double val = p->coeff[0];
  size_t degree = p->deg;
  double x0 = x;

  for (size_t i = 1; i <= degree; ++i) {
    val += p->coeff[i] * x0;
    x0 *= x;
  }

  return val;
}

double evalPolyWrapper(double x) {
  return evalPoly(globalPolynomialPtr, x);
}

// Divide polynomial p by (x - root), and return the resulting polynomial.
// Also, free the memory occupied by p.
Polynomial* deflatePoly(Polynomial*p, double root) {
  Polynomial* q = malloc(sizeof(Polynomial));
  size_t NewDeg = p->deg - 1;
  q->deg = NewDeg;
  q->coeff = malloc((NewDeg + 1) * sizeof(double));
  q->coeff[NewDeg] = p->coeff[p->deg];
  
  for (size_t i = p->deg - 1; i > 0; --i)
    q->coeff[i - 1] = q->coeff[i] * root + p->coeff[i];   

  free(p->coeff);
  free(p);
  return q;
}

// Find all zeros of the polynomial p. Starting point for the iterations: x = 0
void findAllZerosPoly(Polynomial *p) {
  if (!p) {
    printf("Null pointer passed!\n");
    return;
  }

  globalPolynomialPtr = p;
  size_t deg = p->deg;

  for (size_t i = 1; i <= deg; ++i) {
    double root = findZero(evalPolyWrapper, 1.0);
    printf("Root #%zu: %f\n", i, root);
    p = deflatePoly(p, root);
    globalPolynomialPtr = p;
  }
  free(p->coeff);
  free(p);
}

int main(int argc, char* argv[argc + 1]) {
  //complex (*g)(complex) = csin;
  //centeredDiffComplex(g, CMPLX(0.0, 1.0));

  //double (*f)(double) = cos;
  //double x0 = 1.0;
  //double root = findZero(f, x0);
  //printf("Found root of cosine: %f\n", root);

  Polynomial* p = malloc(sizeof(Polynomial));
  p->deg = 3;
  p->coeff = malloc((p->deg + 1) * sizeof(double));
  p->coeff[0] = -3;
  p->coeff[1] = -13.0;
  p->coeff[2] = 0.0;
  p->coeff[3] = 16.0;
  //double val = evalPoly(&p, 2.0);
  //Polynomial* q = deflatePoly(&p, -1.0);
  findAllZerosPoly(p);
  
  //free(p.coeff);
  //free(q->coeff);
  //free(q);
  return 0;
}
