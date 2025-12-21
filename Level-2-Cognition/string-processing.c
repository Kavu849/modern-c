#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Helpers ---
char* my_strdup(const char* s) {
    if (!s) return NULL;
    size_t len = strlen(s) + 1;
    char* copy = malloc(len);
    if (copy) memcpy(copy, s, len);
    return copy;
}

// FIX: Helper to find the correct closing bracket ']'
// It intelligently skips over POSIX classes like [:digit:]
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

// --- Word Processing ---
const char* findWord(const char* s, const char* word) {
  if (!word || !s) return NULL;
  const char* beg_str = s;

  for (const char* c = s; *c; ++c) {
    if ((c != beg_str) && (isalnum((unsigned char)*(c-1))))
      continue;

    const char* cpos = c;
    const char* wpos = word;

    while (*wpos && *cpos == *wpos) {
      ++cpos;
      ++wpos;
    }

    if (!*wpos && (isspace((unsigned char)*cpos) || ispunct((unsigned char)*cpos) || *cpos == '\0')) {
      return c;
    }
  }
  return NULL;
}

char* replaceWord(const char* s, const char* word, const char* replacement) {
  if (!word || !s || !replacement) return NULL;
  const char* found = findWord(s, word);
  if (!found) return my_strdup(s);

  char* newS = malloc(strlen(s) - strlen(word) + strlen(replacement) + 1);
  char* newSBeg = newS;

  while (s != found) *newS++ = *s++;
  while (*word) { s++; word++; }
  while (*replacement) *newS++ = *replacement++;
  while (*s) *newS++ = *s++;
  
  *newS = '\0';
  return newSBeg;
}

// --- Regex Engine ---

const char* matchHere(const char* regex, const char* s);
int matchClass(char c, const char* start, const char* end);

const char* findRegex(const char* s, const char* regex, const char** endPtr) {
  const char* curr = s;
  do {
    const char* e = matchHere(regex, curr);
    if (e) {
      *endPtr = e;
      return curr;
    }
    if (*curr == '\0') break; 
    ++curr;
  } while (1);
  return NULL;
}

const char* matchHere(const char* regex, const char* s) {
  if (regex[0] == '\0') return s;

  if (regex[0] == '(') {
    const char* closing = strchr(regex, ')');
    if (!closing) return NULL;
    return matchHere(regex + 1, s);
  }
  if (regex[0] == ')') return matchHere(regex + 1, s);

  if (regex[0] == '*') { 
    const char* result = matchHere(regex + 1, s);
    if (result) return result;
    if (*s != '\0') return matchHere(regex, s + 1);
    return NULL;
  }

  if (s[0] == '\0') return NULL;

  if (regex[0] == '[') {
    // FIX: Use the smart helper instead of strchr
    const char* end = findClosingBracket(regex + 1);
    
    if (end && matchClass(*s, regex + 1, end))
      return matchHere(end + 1, s + 1);
    return NULL;
  }

  if (regex[0] == '?' || regex[0] == *s)
    return matchHere(regex + 1, s + 1);

  return NULL;
}

int matchClass(char c, const char* start, const char* end) {
  int negate = 0;
  if (*start == '^') { negate = 1; start++; }

  int matched = 0;
  // FIX: Use p < end to prevent overshooting infinite loops
  for (const char* p = start; p < end; ++p) {
    
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

    if ((p + 2) < end && *(p + 1) == '-') {
      if (c >= *p && c <= *(p + 2)) matched = 1;
      p += 2;
      continue;
    }
    
    if (c == *p) matched = 1;
  }
  return negate ? !matched : matched;
}

char* regexReplace(const char* s, const char* regex, const char* replacement) {
    const char* matchEnd = NULL;
    const char* matchStart = findRegex(s, regex, &matchEnd);

    if (!matchStart) return my_strdup(s);

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
