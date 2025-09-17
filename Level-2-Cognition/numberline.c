#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef EFAULT
# define EFAULT EDOM
#endif
#ifndef EOVERFLOW
# define EOVERFLOW (EFAULT-EOF)
# if EOVERFLOW > INT_MAX
#  error EOVERFLOW constant is too large
# endif
#endif
#ifndef ENOMEM
# define ENOMEM (EOVERFLOW+EFAULT-EOF)
# if ENOMEM > INT_MAX
#  error ENOMEM constant is too large
# endif
#endif

static size_t numberline_inner(char const*restrict act,
                               size_t numb[restrict static 1], int base) {
  size_t n = 0;
  for (char* next = nullptr; act[0]; act = next) {
    numb[n] = strtoull(act, &next, base);
    if (act == next) break;
    ++n;
  }
  return n;
}

size_t* numberline(size_t size, char const lbuf[restrict static size],
                   size_t*restrict np, int base) {
  size_t* ret = nullptr;
  size_t n = 0;
  /* Check for validity of the string, first. */
  if (memchr(lbuf, 0, size)) {
    /* The maximum number of integers encoded. */
    ret = malloc(sizeof(size_t[1 + (2 * size) / 3]));
    if (!ret) return nullptr;
    n = numberline_inner(lbuf, ret, base);

    size_t len = n ? n : 1;
    size_t* ret2 = realloc(ret, sizeof(size_t[len]));
    if (ret2) ret = ret2;
  }
  if (np) *np = n;
  return ret;
}

char* fgetline(size_t size, char s[restrict size], FILE*restrict stream) {
  s[0] = 0;
  char* ret = fgets(s, size, stream);
  if (ret) {
    /* Uses non-const variant of strchr */
    char* pos = strchr(s, '\n');
    if (pos) *pos = 0;
    else ret = nullptr;
  }
  return ret;
}

static inline int error_cleanup(int err, int prev) {
  errno = prev;
  return -err;
}

int sprintnumbers(size_t tot, char buf[restrict tot],
                  char const form[restrict static 1],
                  char const sep[restrict static 1],
                  size_t len, size_t nums[const restrict static len]) {
  char* p = buf;  /* next position in buf */
  size_t const seplen = strlen(sep);
  if (len) {
    size_t i = 0;
    for (; i < len;) {
      p += sprintf(p, form, nums[i]);
      ++i;
      if (i >= len) break;
      memcpy(p, sep, seplen);
      p += seplen;
    }
  }
  memcpy(p, "\n", 2);
  return (p - buf) + 1;
}

int fprintnumbers(FILE*restrict stream, char const form[restrict static 1],
                  char const sep[restrict static 1], size_t len,
                  size_t nums[restrict len]) {
  if (!stream)        return -EFAULT;
  if (len && !nums)   return -EFAULT;
  if (len > INT_MAX)  return -EOVERFLOW;

  size_t tot = (len ? len : 1) * strlen(sep);
  int err = errno;
  char* buf = nullptr;

  if (len) {
    /* Count the chars for the numbers. */
    for (size_t i = 0; i < len; ++i)
      tot += snprintf(nullptr, 0, form, nums[i]);
    /* We return int so we have to constrain the max size. */
    if (tot > INT_MAX) return error_cleanup(EOVERFLOW, err);
  }

  buf = malloc(tot + 1);
  if (!buf) return error_cleanup(ENOMEM, err);

  sprintnumbers(tot, buf, form, sep, len, nums);
  /* print whole line in one go */
  if (fputs(buf, stream) == EOF) tot = EOF;
  free(buf);
  return tot;
}

int main(void) {
  char lbuf[256];
  for (;;) {
    if (fgetline(sizeof lbuf, lbuf, stdin)) {
      size_t n;
      size_t* nums = numberline(strlen(lbuf) + 1, lbuf, &n, 0);
      if (nums) {
        int ret = fprintnumbers(stdout, "%#zX", ",\t", n, nums);
        if (ret < 0) return EXIT_FAILURE;
        free(nums);
      }
    } else {
      if (lbuf[0]) {  /* a partial line has been read */
        for (;;) {
          int c = getc(stdin);
          if (c == EOF) return EXIT_FAILURE;
          if (c == '\n') {
            fprintf(stderr, "line too long: %s\n", lbuf);
            break;
          }
        }
      } else break;   /* regular end of input */
    }
  }
}
