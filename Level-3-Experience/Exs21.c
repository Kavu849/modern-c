#include <stdio.h>

#define min(A, B)                     \
_Generic((A)+(B),                     \
         float: minf,                 \
         long double: minl,           \
         unsigned int: minui,         \
         unsigned long: minul,        \
         unsigned long long: minull,  \
         signed int:  minsi,          \
         signed long: minsl,          \
         signed long long: minsll,    \
         default: mind) ((A), (B))

static inline double mind(double a, double b) {
  return a < b ? a : b;
}

static inline long double minl(long double a, long double b) {
  return a < b ? a : b;
}

static inline float minf(float a, float b) {
  return a < b ? a : b;
}

static inline unsigned int minui(unsigned int a, unsigned int b) {
  return a < b ? a : b;
}

static inline unsigned long minul(unsigned long a, unsigned long b) {
  return a < b ? a : b;
}

static inline unsigned long long minull(unsigned long long a, unsigned long long b) {
  return a < b ? a : b;
}

static inline signed int minsi(signed int a, signed int b) {
  return a < b ? a : b;
}

static inline signed long minsl(signed long a, signed long b) {
  return a < b ? a : b;
}

static inline signed long long minsll(signed long long a, signed long long b) {
  return a < b ? a : b;
}

int main(int argc, char* argv[argc+1]){
  double a = 1.0;
  double b = -1.0;
  printf("Minimum of %g and %g is %g\n", a, b, min(a, b));
  
  unsigned long long c = 45;
  unsigned long long d = 90;
  printf("Minimum of %d and %d is %d\n", c, d, min(c, d));
  return 0;
}
