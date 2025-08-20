#ifndef RATIONALS_H
# define RATIONALS_H 1
#include <stdbool.h>
#include "euclid.h"

typedef struct rat rat;

struct rat {
  bool sign;
  size_t num;
  size_t denom;
};

#endif
