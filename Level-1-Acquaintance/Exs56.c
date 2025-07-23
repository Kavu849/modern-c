#include <stdio.h>
#include <stdlib.h>

size_t fib(size_t n) [[__unsequenced__]] {
  if (n < 3)
    return 1;
  else
    return fib(n - 1) + fib(n - 2);
}

int main(int argc, char* argv[argc+1]) {
  if (argc < 2) {
    printf("You need to supply the value of 'n' to the program!\n");
    return 1;
  }
  size_t n = (size_t)strtoull(argv[1], nullptr, 10);
  size_t res = fib(n);
  printf("For n = %zu the value of fib(n) equals %zu\n", n, res);
  return 0;
}
