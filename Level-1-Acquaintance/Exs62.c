#include <stdio.h>

int main(int argc, char* argv[argc+1]) {
#if (-1 & 3) == 3
  printf("This is a 2's complement machine.\n");
#elif (-1 & 3) == 2
  printf("This is a 1's complement machine.\n");
#elif (-1 & 3) == 1
  printf("This is a sign-and-magnitude machine.\n");
#else
  printf("The sign representation of this machine is unrecognized!\n");
#endif
  return 0;
}
