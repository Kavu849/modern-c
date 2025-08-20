#include <stdio.h>

void double_swap(double* p0, double* p1, double* p2) {
  auto tmp = *p2;
  *p2 = *p1;
  *p1 = *p0;
  *p0 = tmp;
}


int main(int argc, char* argv[argc + 1]) {
  double x = 1.0;
  double y = 2.0;
  double z = 3.0;

  double *p0 = &x;
  double *p1 = &y;
  double *p2 = &z;

  printf("Values before the swap: x = %f, y = %f, z = %f\n", x, y, z);
  
  double_swap(p0, p1, p2);

  printf("Values after the swap: x = %f, y = %f, z = %f\n", x, y, z);
  return 0;
}
