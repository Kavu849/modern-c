#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

// Program computing pi to N-th decimal place using Chudnovsky's algorithm
// Input: N - decimal places of accuracy of the approximation.
// Output: Printf of the approximation to standard output 


static double const eps1m01 = 1.0 - 0x1P-01; // 0x1P-01 == 0.5
static double const eps1p01 = 1.0 + 0x1P-01;

// Computing the square root using Heron's algorithm, as accurately as possible
double root(double a) { 

  // First, bring the initial estimate closer to 1/a
  double x = 1.0;
  for (;;) {
    double prod = a * x;
    if (prod < eps1m01)
      x *= 2;
    else if (prod > eps1p01)
      x *= 0.5;
    else
      break;
  }

  for (;;) {
    double newX = 0.5 * (x + a / x);
    if (newX == x)
      return newX;
    x = newX;
  }
}

typedef struct PQR {
  double P;
  double Q;
  double R;
} PQR;

PQR binarySplitting(double a, double b) {
  PQR res;
  double Pab, Qab, Rab;

  if (b == (a + 1)) {
    Pab = -(6*a-1)*(2*a-1)*(6*a-5);
    Qab = 10939058860032000.0 * pow(a, 3);
    Rab = Pab * (545140134.0 * a + 13591409.0);
  }
  else {
    uint64_t m = (a + b) / 2;
    PQR ResAM = binarySplitting(a, m);
    PQR ResMB = binarySplitting(m, b);
    Pab = ResAM.P * ResMB.P;
    Qab = ResAM.Q * ResMB.Q;
    Rab = ResMB.Q * ResAM.R + ResAM.P * ResMB.R;
  }
  res.P = Pab;
  res.Q = Qab;
  res.R = Rab;
  return res;
}

double chudnovsky(uint64_t n) {
  PQR res = binarySplitting(1, n);
  double sq = root(10005.0);
  double pi = (426880 * sq * res.Q) / (13591409 * res.Q + res.R);
  return pi;
}

int main(int argc, char* argv[argc + 1]) {
  int N = atoi(argv[1]);
  printf("Computing pi with %d digits of accuracy.\n", N);
  uint64_t i = 2;
  double pi;

  for (;;) {
    pi = chudnovsky(i);
    if ((uint64_t)(pi * pow(10, N)) == (uint64_t)(M_PI * pow(10, N)))
      break;
    ++i;
  }
  printf("The approximation of pi was obtained for i = %lu\n", i);
  printf("Obtained approximation: pi = %.*f\n", N, pi);
  return 0;
}
