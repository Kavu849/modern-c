#include <stdio.h>

#define static_assert_compatible(A, B, REASON)                          \
  static_assert(_Generic((typeof(A)*)nullptr,                           \
                         typeof(B)*: _Generic((typeof((A)+0)*)nullptr,  \
                                              typeof(A)*: true,         \
                                              default: false),          \
                         default: false),                               \
                "expected compatible types: " REASON ", have " #A " and " #B "")

#define SWAP(X, Y)                                              \
  do {                                                          \
    /* These two variables have the role of function            \
       parameters. They ensure to evaluate the                  \
       expression X and Y once, since these could be            \
       complicated lvalue expressions with valuated             \
       subexpressions that have side effects. */                \
    auto const swap_p1 = &(X);                                  \
    auto const swap_p2 = &(Y);                                  \
    static_assert_compatible(*swap_p1, *swap_p2,                \
                             "to exchange values, '"            \
                             #X "' and '" #Y                    \
                             "' must have compatible types");   \
    auto swap_tmp = *swap_p1;                                   \
    *swap_p1 = *swap_p2;                                        \
    *swap_p2 = swap_tmp;                                        \
  } while (false)

int main() {
  int x = 1;
  int y = 2;
  printf("Before the swap: x = %d, y = %d\n", x, y);
  SWAP(x, y);
  printf("After the swap: x = %d, y = %d\n", x, y);
  
  long z = 3;
  //SWAP(x, z); // This should fail the static assert with the correct message
  
  int arr1[10];
  int arr2[10];
  SWAP(arr1, arr2); // This should fail the static assert (is array)

  return 0;
}
