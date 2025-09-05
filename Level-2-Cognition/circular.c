#include <stdlib.h>
#include <string.h>
#include "circular.h"

[[deprecated("implementation")]]
circular* circular_init(circular* c, size_t cap) {
  if (c) {
    if (cap) {
      *c = (circular) {
        .cap = cap,
        .tab = malloc(sizeof(double[cap])),
      };
      // Allocation failed.
      if (!c->tab) c->cap = 0;
    } else {
      *c = (circular) { };
    }
  }
  return c;
}

[[deprecated("implementation")]]
void circular_destroy(circular* c) {
  if (c) {
    free(c->tab);
    circular_init(c, 0);
  }
}

[[deprecated("implementation")]]
static size_t circular_getpos(circular const c[static 1], size_t pos) {
  if (c->cap) {
    pos += c->start;
    pos %= c->cap;
  }
  return pos;
}

[[deprecated("implementation")]]
double* circular_element(circular const* c, size_t pos) {
  double* ret = nullptr;
  if (c) {
    if (pos < c->cap) {
      pos = circular_getpos(c, pos);
      ret = &c->tab[pos];
    }
  }
  return ret;
}

circular* circular_append(circular* c, double value) {
  circular* ret = nullptr;
  if (c) {
    double* where = circular_element(c, c->len);
    if (where) {
      *where = value;
      c->len += 1;
      ret = c;
    }
  }
  return ret;
}

double circular_pop(circular* c) {
  double ret = 0.0;
  if (c && c->len) {
    double* pos = circular_element(c, 0);
    if (pos) ret = *pos;
    c->len -= 1;
    c->start = (c->start + 1) % c->cap;
  }
  return ret;
}

size_t circular_getlength(circular const* c) {
  size_t ret = 0;
  if (c)
    ret = c->len;
  return ret;
}

[[nodiscard("returned pointer replaces function argument")]]
[[deprecated("implementation")]]
circular* circular_resize(circular* c, size_t nlen) {
  if (c) {
    size_t len = c->len;
    if (len > nlen) return nullptr;
    size_t olen = c->cap;
    if (nlen != olen) {
      size_t ostart = circular_getpos(c, 0);
      size_t nstart = ostart;
      double* otab = c->tab;
      double* ntab;
      if (nlen > olen) {
        ntab = realloc(c->tab, sizeof(double[nlen]));
        if (!ntab) return nullptr;
        // non-empty, and there were already two separate chunks
        if (ostart + len > olen) {
          size_t ulen = olen - ostart;
          size_t llen = len - ulen;
          if (llen <= (nlen - olen)) {
            /* Copy the lower one up after the old end. */
            memcpy(ntab + olen, ntab, llen * sizeof(double));
          } else {
            /* Move the upper one up to the new end. */
            nstart = nlen - ulen;
            memmove(ntab + nstart, ntab + ostart, ulen * sizeof(double));
          }
        }
      } else {
        if (ostart + len > olen) {
          // Two separate chunks; mv the upper one down to the new end.
          size_t ulen = olen - ostart;
          nstart = nlen - ulen;
          memmove(otab + nstart, otab + ostart, ulen * sizeof(double));
        } else {
          // A single chunk
          if (ostart + len > nlen) {
            // Reallocation cuts the existing chunk in two.
            memmove(otab, otab + ostart, len * sizeof(double));
            nstart = 0;
          }
        }
        if (nlen) {
          // Now all data is saved in the conserved part of the array.
          ntab = realloc(c->tab, sizeof(double[nlen]));
          // If realloc fails in this case (would be weird), just overrule it.
          if (!ntab) ntab = otab;
        } else {
          // There had been no elements and we were asked to reduce the buffer
          // to 0 elements.
          free(c->tab);
          ntab = nullptr;
          nstart = 0;
        }
      }
      *c = (circular){
        .cap = nlen,
        .start = nstart,
        .len = len,
        .tab = ntab,
      };
    }
  }
  return c;
}

int main(void) {
  return 0;
}
