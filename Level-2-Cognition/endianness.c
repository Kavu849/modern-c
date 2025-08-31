#include <stdio.h>
#include <inttypes.h>

typedef union unsignedInspect unsignedInspect;
union unsignedInspect {
  unsigned val;
  unsigned char bytes[sizeof(unsigned)];
};
unsignedInspect twofold = { .val = 0xAABBCCDD, };

typedef union doublePtrInspect doublePtrInspect;
union doublePtrInspect {
  double* val;
  unsigned char bytes[sizeof(double*)];
};
double x = -16.5;
doublePtrInspect doublePtr = { .val = &x, };

int main(void) {
  printf("value is 0x%.08X\n", twofold.val);
  for (size_t i = 0; i < sizeof twofold.bytes; ++i)
    printf("byte[%zu]: 0x%.02hhX\n", i, twofold.bytes[i]);
  unsigned val = 0xAABBCCDD;
  unsigned char* valp = (unsigned char*)&val;
  for (size_t i = 0; i < sizeof val; ++i)
    printf("byte[%zu]: 0x%.02hhX\n", i, valp[i]);

  printf("Value is %p\n", (void*)doublePtr.val);
  for (size_t i = 0; i < sizeof(doublePtr.bytes); ++i)
    printf("byte[%zu]: 0x%.02hhX\n", i, doublePtr.bytes[i]);

  printf("The address of variable x = -16.5: %p\n", (void*)&x);
  printf("Addresses of two consecutive elements of doublePtr.bytes array:\n");
  printf("    &bytes[0]: %p\n", (void*)&doublePtr.bytes[0]);
  printf("    &bytes[1]: %p\n", (void*)&doublePtr.bytes[1]);
  return 0;
}
