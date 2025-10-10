#include "string-processing.h"

#include <stdio.h>
#include <stdlib.h>

const char* findWord(const char* word, const char* s) {
  if (!word || !s)
    return nullptr;

  const char* found = nullptr;

  for (const char* c = s; *c; ++c) {
    const char* cpos = c;
    const char* wpos = word;

    while (*cpos == *wpos) {
      ++cpos;
      ++wpos;
    }

    if (!*wpos) {
      found = c;
      break; // return the beginning of 'word' in 's'
    }
  }

  //if (found == s) 
  if (!found)
    return nullptr;
  
  return found;
}

int main(int argc, char* argv[argc+1]) {
  char str[] = "This is a string for testing purposes\n";
  printf("%s", str);
  const char word[] = "is";
  const char* find = findWord(word, str);
  if (find)
    printf("%c\n", *find);
  return EXIT_SUCCESS;
}
