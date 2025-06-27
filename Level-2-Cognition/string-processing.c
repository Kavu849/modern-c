#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* myStrdup(const char* s) {
    if (!s) return NULL;
    size_t len = strlen(s) + 1;
    char* copy = malloc(len);
    if (copy) memcpy(copy, s, len);
    return copy;
}

// Helper function for finding the closing bracker for a character class
const char* findClosingBracket(const char* regex) {
    const char* p = regex;
    while (*p) {
        // If we see "[:", skip until ":]"
        if (*p == '[' && *(p+1) == ':') {
            const char* posix_end = strstr(p, ":]");
            if (posix_end) {
                p = posix_end + 2; // Jump past ":]"
                continue;
            }
        }
        // Found the real closing bracket
        if (*p == ']') {
            return p;
        }
        p++;
    }
    return NULL;
}

const char* findWord(const char* s, const char* word) {
  if (!word || !s) return NULL;
  const char* beg_str = s;

  for (const char* c = s; *c; ++c) {
    // Find the beginning of a word (either beginning of the string, or the
    // previous character is not alphanumeric)
    if ((c != beg_str) && (isalnum((unsigned char)*(c-1))))
      continue;

    const char* cpos = c;
    const char* wpos = word;
    
    // Check if the characters match the word that we are lookng for
    while (*wpos && *cpos == *wpos) {
      ++cpos;
      ++wpos;
    }
    
    // If we reached the end of the word and the character after that in the
    // string is either a space, punctuatin, or the end of the string, we have
    // a match
    if (!*wpos && (isspace((unsigned char)*cpos) || ispunct((unsigned char)*cpos) || *cpos == '\0')) {
      return c;
    }
  }
  return NULL;
}

char* replaceWord(const char* s, const char* word, const char* replacement) {
  if (!word || !s || !replacement) return NULL;
  const char* found = findWord(s, word);
  // If we did not find the word, return a malloced copy of s using myStrdup
  if (!found) return myStrdup(s);

  // Malloc memory for the new string which will be returned
  char* newS = malloc(strlen(s) - strlen(word) + strlen(replacement) + 1);
  char* newSBeg = newS;

  // Until the found word, new and old strings will be the same
  while (s != found)
    *newS++ = *s++;

  // Fast-forward 'word' in the old string
  while (*word) {
    s++;
    word++;
  }

  // Place the new word in the new string
  while (*replacement)
    *newS++ = *replacement++;

  // The rest of the new string will be the same as the rest of the old string
  while (*s)
    *newS++ = *s++;
  
  // Add the null-terminator and return the beginning of the new string
  *newS = '\0';
  return newSBeg;
}

const char* matchHere(const char* regex, const char* s);
int matchClass(char c, const char* start, const char* end);

// Find regex in string s
const char* findRegex(const char* s, const char* regex, const char** endPtr) {
  const char* curr = s;
  while (1) {
    // Match regex at position curr
    const char* e = matchHere(regex, curr);
    // If we found a match, return the beginning position curr, and save the
    // end position in endPtr
    if (e) {
      *endPtr = e;
      return curr;
    }
    // If we reached the end of the string, break
    if (*curr == '\0') break; 
    ++curr;
  }
  return NULL;
}

// Recursive function matching a regex with a string
const char* matchHere(const char* regex, const char* s) {
  // If we reached the end of regex, we found a match
  if (regex[0] == '\0')
    return s;

  // Grouping case
  if (regex[0] == '(') {
    const char* closing = strchr(regex, ')');
    if (!closing) return NULL;
    // Move to the next character of the regex
    return matchHere(regex + 1, s);
  }
  if (regex[0] == ')') return matchHere(regex + 1, s);

  // '*' matches anything
  if (regex[0] == '*') { 
    // Move regex one character forward, leave s as it is (matching zero
    // characters)
    const char* result = matchHere(regex + 1, s);
    if (result) return result;
    // Leave regex as it is, move the string forward one character (matching
    // one character from s
    if (*s != '\0') return matchHere(regex, s + 1);
    return NULL;
  }

  // If we reached the end of the string, no match found
  if (s[0] == '\0') return NULL;

  // Match a character class
  if (regex[0] == '[') {
    const char* end = findClosingBracket(regex + 1);
    
    // If we match the class, move the regex and the string foward by one
    if (end && matchClass(*s, regex + 1, end))
      return matchHere(end + 1, s + 1);
    return NULL;
  }

  // '?' matches any character. The second condition in the 'if' statement
  // performs a regular charater match
  if (regex[0] == '?' || regex[0] == *s)
    return matchHere(regex + 1, s + 1);

  return NULL;
}

// Helper function matching character c to a character class defined between
// start and end. Start points to the first character after '[', and end points
// to ']'
int matchClass(char c, const char* start, const char* end) {
  int negate = 0;
  // '^' negates the character class
  if (*start == '^') {
    negate = 1;
    start++;
  }

  int matched = 0;
  for (const char* p = start; p < end; ++p) {
    // Match various POSIX character classes
    if (*p == '[' && *(p + 1) == ':') {
      if (strncmp(p, "[:alpha:]", 9) == 0) {
        if (isalpha((unsigned char)c)) matched = 1; 
        p += 8; 
      } 
      else if (strncmp(p, "[:digit:]", 9) == 0) {
        if (isdigit((unsigned char)c)) matched = 1; 
        p += 8;
      } 
      else if (strncmp(p, "[:alnum:]", 9) == 0) {
        if (isalnum((unsigned char)c)) matched = 1; 
        p += 8;
      } 
      else if (strncmp(p, "[:space:]", 9) == 0) {
        if (isspace((unsigned char)c)) matched = 1; 
        p += 8;
      }
      continue;
    }

    // Match characters in a range (e.g. [A-Z])
    if ((p + 2) < end && *(p + 1) == '-') {
      if (c >= *p && c <= *(p + 2)) matched = 1;
      p += 2;
      continue;
    }
    
    // Match a single character
    if (c == *p) matched = 1;
  }
  return negate ? !matched : matched;
}

// Replace regex (if found in s) with replacement and return a new malloced
// string
char* regexReplace(const char* s, const char* regex, const char* replacement) {
    const char* matchEnd = NULL;
    const char* matchStart = findRegex(s, regex, &matchEnd);

    if (!matchStart) return myStrdup(s);

    size_t prefixLen = matchStart - s;
    size_t replaceLen = strlen(replacement);
    size_t suffixLen = strlen(matchEnd);
    
    char* newS = malloc(prefixLen + replaceLen + suffixLen + 1);
    if (!newS) return NULL;

    char* ptr = newS;
    memcpy(ptr, s, prefixLen); ptr += prefixLen;
    memcpy(ptr, replacement, replaceLen); ptr += replaceLen;
    strcpy(ptr, matchEnd);

    return newS;
}

int main(int argc, char* argv[]) {
  char text[] = "User: alice123, ID: 9942, Code: X-55";
  printf("Original: %s\n\n", text);

  // Test 1: POSIX Digit Match
  const char* end = NULL;
  const char* start = findRegex(text, "[[:digit:]][[:digit:]]", &end);
  if (start) {
    printf("Test 1 (POSIX): Found digits '%.*s'\n", (int)(end-start), start);
  } else {
    printf("Test 1: Failed\n");
  }

  // Test 2: Negated Class
  start = findRegex(text, "[^a-zA-Z0-9 ]", &end);
  if (start) {
    printf("Test 2 (Negated): Found symbol '%.*s'\n", (int)(end-start), start);
  }

  // Test 3: Glob Replace
  char* result = regexReplace(text, "ID: *,", "ID: HIDDEN,");
  if (result) {
      printf("Test 3 (Replace): %s\n", result);
      free(result);
  }

  return 0;
}
