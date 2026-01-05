#ifndef RATIONALS_H
# define RATIONALS_H 1
#include <stdbool.h>
#include "euclid.h"

typedef struct rat rat;

struct rat {
  bool sign; // sign == true iff '-'
  size_t num;
  size_t denom;
};

/* Functions that return a value of type rat. */
inline rat rat_get(signed sign, size_t num, size_t denom);
inline rat rat_get_normal(rat x);
inline rat rat_get_extended(rat x, size_t f);
inline rat rat_get_prod(rat x, rat y);
inline rat rat_get_sum(rat x, rat y);


/* Functions that operate on pointers to rat. */
void rat_destroy(rat* rp);
rat* rat_init(rat* rp, signed sign, size_t num, size_t denom);
rat* rat_normalize(rat* rp);
rat* rat_extend(rat* rp, size_t f);
rat* rat_sumup(rat* rp, rat y);
rat* rat_rma(rat* rp, rat x, rat y);

/* Functions that are implemented as exercises. */
/** @brief Printf @a x into @a tmp and return tmp. **/
char const* rat_print(size_t len, char tmp[len], rat const* x);
/** @brief Print @a x normalize and print. **/
char const* rat_normalize_print(size_t len, char tmp[len], rat const* x);
rat* rat_dotproduct(rat rp[static 1], size_t n, rat const A[n], rat const B[n]);

#endif
