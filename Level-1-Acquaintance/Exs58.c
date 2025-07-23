#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Compute Fibonacci number n with the help of a cache that may hold previously
   computed values. */

size_t fibCacheRec(size_t n, size_t cache[static n]) {
  if (!cache[n - 1]) {
    cache[n - 1] = fibCacheRec(n - 1, cache) + fibCacheRec(n - 2, cache);
  }
  return cache[n - 1];
}

size_t fibCache(size_t n) {
  if (n < 2) return 1;
  /* Set up a VLA to cache the values. */
#if __STDC_VERSION__ > 202311L
  /* Since C23, VLA can be default initialized. */
  size_t cache[n] = { };
#else
  size_t cache[n]; memset(cache, 0, n*sizeof(*cache));
#endif
  /* Non-trivial initialization is replaced by assignment. */
  cache[0] = 1; cache[1] = 1;
  /* Call the recursive function. */
  return fibCacheRec(n, cache);
}


int main(int argc, char* argv[argc+1]) {
  if (argc < 2) {
    printf("You need to supply the value of 'n' to the program!\n");
    return 1;
  }

  size_t n = (size_t)strtoull(argv[1], nullptr, 10);
  size_t res = fibCache(n);
  printf("For n = %zu the value of fibCache(n) equals %zu\n", n, res);
  return 0;
}
