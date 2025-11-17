#include "string-processing.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* findWord(const char* s, const char* word) {
  if (!word || !s)
    return NULL;

  const char* found = NULL;
  const char* beg_str = s;

  for (const char* c = s; *c; ++c) {
    // Make sure to start at the beginning of a word
    if ((c != beg_str) && (!isspace(*(c-1))))
      continue;

    const char* cpos = c;
    const char* wpos = word;

    while (*cpos == *wpos) {
      ++cpos;
      ++wpos;
    }

    if (!*wpos && (isspace(*cpos) || ispunct(*cpos) || *cpos == '\0')) {
      found = c;
      break; // return the beginning of 'word' in 's'
    }
  }

  if (!found)
    return NULL;
  
  return found;
}

char* replaceWord(const char* s, const char* word, const char* replacement) {
  if (!word || !s || !replacement)
    return NULL;

  const char* found = findWord(s, word);

  if (!found)
    return strdup(s);

  // Create a new string, which we use to compose the string after replacement
  char* newS = malloc(strlen(s) - strlen(word) + strlen(replacement) + 1);
  char* newSBeg = newS;

  while (s != found) {
    *newS = *s;
    ++s;
    ++newS;
  }

  while (*word) {
    ++s;
    ++word;
  }

  while (*replacement) {
    *newS = *replacement;
    ++replacement;
    ++newS;
  }

  while (*s) {
    *newS = *s;
    ++s;
    ++newS;
  }

  *newS = '\0';
  return newSBeg;
}

int main(int argc, char* argv[argc+1]) {
  char str[] = "This is a string for testing purposes\n";
  printf("%s", str);
  const char word[] = "is";
  const char* found = findWord(str, word);
  if (found) {
    printf("Found the word: \"%s\", as \"", word);
    const char* it = word;
    while (*it) {
      printf("%c", *it);
      ++it;
    }
    printf("\"\n");

    char replacement[] = "verifying";
    printf("Replacing \"%s\" with \"%s\"\n", word, replacement);
    char* newS = replaceWord(str, word, replacement);
    printf("String after replacement: %s \n", newS);
  } else
    printf("Didn't find the word: %s \n", word);
  return EXIT_SUCCESS;
}
