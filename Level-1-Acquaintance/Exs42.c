#include <stdio.h>

int main(int argc, char* argv[argc+1]) {
  double x = 1.0E-13 + 1.0E-13;
  double y = (1.0E-13 + (1.0E-13 + 1.0)) - 1.0;

  printf("x = %g, y = %g\n", x, y);
}
