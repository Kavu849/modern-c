#include <stdbool.h>
#include <stdio.h>

#define is_null_pointer_constant(X)     \
_Generic(true ? (struct toto*)0 : (X),  \
         struct toto*: 1,               \
         default: 0)

#define is_zero_ice(X) is_null_pointer_constant((void*)(X))

#define isice(X) (is_zero_ice(X) || is_zero_ice(!(X)))

int main() {
  int x = 0;

  printf("Is 0 a constant? %d\n", is_null_pointer_constant(0));
  printf("Is void* a constant? %d\n", is_null_pointer_constant((void*)&x));
  printf("Is NULL a constant? %d\n", is_null_pointer_constant(NULL));

  printf("Is 1 an ICEV0? %d\n", is_zero_ice(1));
  printf("Is 0 an ICEV0? %d\n", is_zero_ice(0));

  printf("Is 2 an ICE? %d\n", isice(2));
  printf("Is x an ICE? %d\n", isice(x));
  printf("Is 0 an ICE? %d\n", isice(0));
  printf("Is &x an ICE? %d\n", isice(&x));

  return 0;
}
