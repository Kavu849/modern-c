#include <stdio.h>
#include <stdlib.h>

size_t fib2iter(size_t n) {
  size_t buf[2] = { 1, 1, };
  size_t res = 1;
  for (size_t i = 3; i <= n; ++i) {
    res = buf[0] + buf[1];
    buf[1] = buf[0];
    buf[0] = res;
  }
  return res;
}

void fib2rec(size_t n, size_t buf[static 2]) [[__unsequenced__]] {
  if (n > 2) {
    size_t res = buf[0] + buf[1];
    buf[1] = buf[0];
    buf[0] = res;
    fib2rec(n - 1, buf);
  }
}

size_t fib2(size_t n) [[__unsequenced__]] {
  size_t res[2] = { 1, 1, };
  fib2rec(n, res);
  return res[0];
}

int main(int argc, char* argv[argc+1]) {
  if (argc < 2) {
    printf("You need to supply the value of 'n' to the program!\n");
    return 1;
  }
  
  size_t const n = strtoull(argv[1], nullptr, 10);
  size_t res_fib2rec = fib2(n);
  size_t res_fib2iter = fib2iter(n);
  printf("For n = %zu the value of fib2rec(n) equals %zu, and the value of "
         "fib2iter(n) equals %zu\n", n, res_fib2rec, res_fib2iter);
  return 0;
}
