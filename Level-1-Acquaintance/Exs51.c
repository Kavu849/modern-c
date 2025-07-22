#include <stdio.h>

struct order1 {
  unsigned char a;
  unsigned b;
  unsigned long long c;
};

struct order2 {
  unsigned char a;
  unsigned long long c;
  unsigned b;
};

struct order3 {
  unsigned b;
  unsigned char a;
  unsigned long long c;
};

struct order4 {
  unsigned b;
  unsigned long long c;
  unsigned char a;
};

struct order5 {
  unsigned long long c;
  unsigned char a;
  unsigned b;
};

struct order6 {
  unsigned long long c;
  unsigned b;
  unsigned char a;
};

int main(int argc, char* argv[argc+1]) {
  size_t uns_char_size = sizeof(unsigned char);
  size_t uns_size = sizeof(unsigned);
  size_t uns_long_long_size = sizeof(unsigned long long);
  size_t min_size = uns_char_size + uns_size + uns_long_long_size;

  printf("Sizes in bytes: unsigned char: %zu, unsigned: %zu, unsigned long long: %zu,"
         " so the minimum size is %zu bytes.\n", uns_char_size, uns_size,
         uns_long_long_size, min_size);
  printf("Sizes of structs:\n");
  printf("unsigned char, unsigned, unsigned long long: %zu bytes\n", sizeof(struct order1));
  printf("unsigned char, unsigned long long, unsigned: %zu bytes\n", sizeof(struct order2));
  printf("unsigned, unsigned char, unsigned long long: %zu bytes\n", sizeof(struct order3));
  printf("unsigned, unsigned long long, unsigned char: %zu bytes\n", sizeof(struct order4));
  printf("unsigned long long, unsigned char, unsigned: %zu bytes\n", sizeof(struct order5));
  printf("unsigned long long, unsigned, unsigned char: %zu bytes\n", sizeof(struct order6));

  return 0;
}
