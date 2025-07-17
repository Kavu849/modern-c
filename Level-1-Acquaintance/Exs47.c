#include <stdio.h>
#include <string.h>

signed safe_strcmp(char const s0[static 1], size_t n0, char const s1[static 1],
                   size_t n1) {
  // Check if s0 is null-terminated
  void* found0 = memchr(s0, '\0', n0);
  void* found1 = memchr(s1, '\0', n1);

  if (found0 && found1)
    return strcmp(s0, s1);

  if (found0) { // s0 is null-terminated, but s1 is not. Need to compare up to
                // the minimum of lengths of s0 and s1
    size_t len0 = (size_t)((char*)found0 - s0);
    if (len0 < n1)
      return memcmp(s0, s1, len0);
    return memcmp(s0, s1, n1);
  }
  
  if (found1) { // s0 is not null-terminated, but s1 is.
    size_t len1 = (size_t)((char*)found1 - s1);
    if (len1 < n0)
      return memcmp(s0, s1, len1);
    return memcmp(s0, s1, n0);
  }
  
  // Otherwise, both are not null-terminated
  if (n0 < n1)
    return memcmp(s0, s1, n0);
  return memcmp(s0, s1, n1);
}

int main(int argc, char* argv[argc+1]) {
  return 0;
}
