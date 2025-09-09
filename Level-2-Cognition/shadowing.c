#include <stdio.h>

unsigned i = 1;

int main(void) {
  unsigned i = 2;       /* A new object */
  if (i) {
    extern unsigned i;  /* An existing object */
    printf("%u\n", i);  /* prints 1 */
  } else {
    printf("%u\n", i);  /* prints 2 */
  }
}
