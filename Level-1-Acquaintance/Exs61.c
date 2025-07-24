#include <stdio.h>

double my_strtod(char const s[static 1]) {
  size_t i = 0;
  // check for +/-
  bool isNegative = false;
  switch (s[i]) {
  case '-':
    isNegative = true;
    ++i;
  case '+':
    ++i;
  }

  double res = 0.0;
  unsigned c;

  while (s[i]) { // evaluate the integer part
    if (s[i] == '.') {
      ++i;
      break;
    }
    c = s[i] - '0';
    res *= 10;
    res += c;
    ++i;
  }

  unsigned base = 10;
  while (s[i]) { // evaluate the fractional part
    c = s[i] - '0';
    res += c / base;
    base /= 10;
  }

  if (isNegative)
    res = -res;

  return res;
}

int main(int argc, char* argv[argc+1]) {
  if (argc < 2) {
    printf("You need to supply the value to convert to the program!\n");
    return 1;
  }

  double res = my_strtod(argv[1]);
  printf("Result: %f\n", res);
  return 0;
}
