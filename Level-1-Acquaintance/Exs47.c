#include <stdio.h>
#include <string.h>

signed safeStrcmp(char const s0[static 1], size_t n0, char const s1[static 1],
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
  // Test the functionality of safeStrcmp
  signed res1 = safeStrcmp("apple", 6, "apple", 6);
  printf("Comparison of \"apple\" and \"apple\" results in %d\n", res1);
  signed res2 = safeStrcmp("apples", 7, "apple", 6);
  printf("Comparison of \"apples\" and \"apple\" results in %d\n", res2);
  signed res3 = safeStrcmp("zebra", 6, "yak", 4);
  printf("Comparison of \"zebra\" and \"yak\" results in %d\n", res3);
  signed res4 = safeStrcmp("apple", 6, "apply", 6);
  printf("Comparison of \"apple\" and \"apply\" results in %d\n", res4);

  char bufferNoNull[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
  char bufferShort[5] = "abc";

  signed res5 = safeStrcmp(bufferNoNull, 8, bufferShort, 4);
  printf("Comparison of non null-terminated bufferNoNull and null-terminated"
         " bufferShort results in %d\n", res5);
  
  return 0;
}
