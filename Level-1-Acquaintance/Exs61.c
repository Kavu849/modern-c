#include <stdio.h>

double my_strtod(char const s[static 1]) {
  size_t i = 0;
  // check for +/-
  bool isNegative = false;
  switch (s[i]) {
  case '-':
    isNegative = true;
    ++i;
    break;
  case '+':
    ++i;
    break;
  default:
    break;
  }

  double res = 0.0;
  unsigned c;

  while (s[i]) { // evaluate the integer part
    if (s[i] == '.') {
      ++i;
      break;
    }
    if (s[i] < '0' || s[i] > '9') {
      printf("Incorrect character encountered!\n");
      break;
    }
    c = s[i] - '0';
    res *= 10;
    res += c;
    ++i;
  }

  double fracMultiplier = 0.1;
  while (s[i]) { // evaluate the fractional part
    if (s[i] < '0' || s[i] > '9') {
      printf("Incorrect character encountered!\n");
      break;
    }
    c = s[i] - '0';
    res += c * fracMultiplier;
    fracMultiplier /= 10;
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
