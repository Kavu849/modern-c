#include <stdio.h>
#include <stdlib.h>

size_t smallestPrimeFactor(size_t n) {
  if (n % 2 == 0)
    return 2;
  
  for (size_t i = 3; i * i <= n; i += 2) {
    if (n % i == 0)
      return i;
  }
  // Otherwise, n itself must be prime
  return n;
}

void findPrimeFactors(size_t n) {
  printf("Prime factors of %zu:", n);
  if (n == 0) {
    printf(" undefined, cannot factor 0\n");
    return;
  }
  if (n == 1) {
    printf(" 1\n");
    return;
  }

  while (n != 1) {
    size_t factor = smallestPrimeFactor(n);
    printf(" %zu", factor);
    n /= factor;
  }
  printf("\n");
}

int main(int argc, char* argv[argc+1]) {
  if (argc < 2) {
    printf("You need to supply the number to factorize to the program!\n");
    return 1;
  }

  size_t const n = strtoull(argv[1], nullptr, 10);
  findPrimeFactors(n);
  return 0;
}
