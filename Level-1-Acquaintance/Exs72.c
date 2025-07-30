#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define INVALID_HEXATRIDECIMAL_CHAR ((unsigned)-1)

/* Supposes that lowercase characters are contiguous */
static_assert('z'-'a' == 25, "alphabetic characters not contiguous");

const unsigned char_to_hexatridecimal_map[256] = {
  [0 ... 255] = INVALID_HEXATRIDECIMAL_CHAR,
  
  // Digits '0' through '9'
  ['0'] = 0, ['1'] = 1, ['2'] = 2, ['3'] = 3, ['4'] = 4, ['5'] = 5, ['6'] 6,
  ['7'] = 7, ['8'] = 8, ['9'] = 9,
  
  // Uppercase 'A' through 'Z'
  ['A'] = 10, ['B'] = 11, ['C'] = 12, ['D'] = 13, ['E'] = 14, ['F'] = 15,
  ['G'] = 16, ['H'] = 17, ['I'] = 18, ['J'] = 19, ['K'] = 20, ['L'] = 21,
  ['M'] = 22, ['N'] = 23, ['O'] = 24, ['P'] = 25, ['Q'] = 26, ['R'] = 27,
  ['S'] = 28, ['T'] = 29, ['U'] = 30, ['V'] = 31, ['W'] = 32, ['X'] = 33,
  ['Y'] = 34, ['Z'] = 35
};

/* Converts an alphanumeric digit to an unsigned */
/* '0' ... '9'  =>  0 ..  9u */
/* 'A' ... 'Z'  => 10 .. 35u */
/* 'a' ... 'z'  => 10 .. 35u */
/* Other values =>   Greater */
unsigned hexatridecimal(int a) {
  if (isdigit(a)) {
    /* This is guaranteed to work: decimal digits
       are consecutive, and isdigit is not
       locale dependent. */
    return a - '0';
  } else {
    /* Leaves a unchanged if it is not lowercase */
    a = toupper(a);
    /* Returns value >= 36 if not Latin uppercase */
    return char_to_hexatridecimal_map[a];
  }
}

int main(int argc, char* argv[argc+1]) {
  for (size_t i = 1; i < argc; ++i) {
    int a = (unsigned char)argv[i][0];
    unsigned h = hexatridecimal(a);
    printf("Argument: a = %c, hexatridecimal(a): %u \n", (char)a, h);
  }
  return 0;
}
