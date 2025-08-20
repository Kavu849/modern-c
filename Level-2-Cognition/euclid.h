#ifndef EUCLID_H
# define EUCLID_H 1

#include <stdio.h>
#include <assert.h>
#include <stddef.h>

inline size_t gcd2(size_t a, size_t b) [[__unsequenced__]] {
  assert(a <= b);
  if (!a) return b;
  size_t r = b % a;
  return gcd2(r, a);
}

inline size_t gcd(size_t a, size_t b) [[__unsequenced]] {
  assert(a);
  assert(b);
  if (a < b)
    return gcd2(a, b);
  else
    return gcd2(b, a);
}

#endif
