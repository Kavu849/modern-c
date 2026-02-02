#include <stdio.h>

#define isnegative(X) ((X) < 0)

#define MAX(X,Y)                                          \
  /* This starts the compound expression construct. */    \
  ({                                                      \
    auto const max_x = (X);                               \
    auto const max_y = (Y);                               \
    /* now the body starts */                             \
    ((isnegative(max_x) && !isnegative(max_y))            \
     ? max_y                                              \
     : ((isnegative(max_y) && !isnegative(max_x))         \
        ? max_x                                           \
        : /* both have the same sign */                   \
        ((max_x < max_y) ? max_y : max_x)));              \
  })

int main() {
  int x = -1;
  int y = 1;
  printf("The max of %d and %d is %d\n", x, y, MAX(x,y));
  y = 0;
  printf("The max of %d and %d is %d\n", x, y, MAX(x,y));
  long z = -20;
  printf("The max of %d and %ld is %ld\n", x, z, MAX(x,z));
  return 0;
}
