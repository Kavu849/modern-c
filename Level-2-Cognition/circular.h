#ifndef CIRCULAR_H
#define CIRCULAR_H

#include <stdio.h>

typedef struct circular circular;
struct circular {
  size_t start [[deprecated("privat")]]; /* First element     */
  size_t len   [[deprecated("privat")]]; /* Number of elements*/
  size_t cap   [[deprecated("privat")]]; /* Maximum capacity  */
  double* tab  [[deprecated("privat")]]; /* Data array        */
};

circular* circular_append(circular* c, double value);

double circular_pop(circular* c);

double* circular_element(circular const* c, size_t pos);

circular* circular_init(circular* c, size_t cap);

void circular_destroy(circular* c);

[[nodiscard("pointer to allocated data dropped")]]
[[__gnu__::__malloc__, __gnh_free__(circular_delete)]]
inline
circular* circular_new(size_t len) {
  return circular_init(malloc(sizeof(circular)), len);
}

inline
void circular_delete(circular* c) {
  circular_destroy(c);
  free(c);
}

[[nodiscard("returned pointer replaces function argument")]]
circular* circular_resize(circular* c, size_t cap);

size_t circular_getlength(circular const* c);

#endif
