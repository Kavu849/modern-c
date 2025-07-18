#include <stdio.h>
#include <string.h>

signed safe_strcmp(char const s0[static 1], size_t n0, char const s1[static 1],
                   size_t n1) {
  // Check if s0 is null-terminated
  void* found0 = memchr(s0, '\0', n0);
  void* found1 = memchr(s1, '\0', n1);
  size_t effLen0 = found0 ? (size_t)((char*)found0 - s0) : n0;
  size_t effLen1 = found1 ? (size_t)((char*)found1 - s1) : n1;
  size_t minEffLen = effLen0 < effLen1 ? effLen0 : effLen1;
  
  signed cmpRes = memcmp(s0, s1, minEffLen);

  if (cmpRes != 0)
    return cmpRes;

  if (effLen0 < effLen1)
    return -1;
  if (effLen1 < effLen0)
    return 1;
  return 0;
}

int main(int argc, char* argv[argc+1]) {
  return 0;
}
