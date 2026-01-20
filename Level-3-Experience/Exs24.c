#include <float.h>
#include <limits.h>
#include <stdio.h>

#define MINVAL(X)                                 \
_Generic((X),                                     \
         bool: (bool) +0,                         \
         char: (char) +CHAR_MIN,                  \
         signed char: (signed char) +SCHAR_MIN,   \
         unsigned char: (unsigned char) +0,       \
         signed short: (signed short) +SHRT_MIN,  \
         unsigned short: (unsigned short) +0,     \
         signed: INT_MIN,                         \
         unsigned: 0U,                            \
         signed long: LONG_MIN,                   \
         unsigned long: 0UL,                      \
         signed long long: LLONG_MIN,             \
         unsigned long long: 0ULL,                \
         float: -FLT_MAX,                         \
         double: -DBL_MAX,                        \
         long double: -LDBL_MAX)

int main(void) {
  double x;
  long y;
  char z;

  printf("Minimum double is %g, minimum long is %ld, minimum char is %d\n", MINVAL(x), MINVAL(y), MINVAL(z));
  return 0;
}
