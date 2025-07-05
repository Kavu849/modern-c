#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Read a full line from stream and return a pointer to it. The function
// automatically resizes the buffer as needed and returns a null pointer
// in case of failure
char* fgetline(FILE* stream) {
  size_t size = 256; // size of allocation (will be doubled every iteration)
  size_t len = 0; // length of characters read so far
  char* line = malloc(size);

  for (;;) {
    if (!fgets(line + len, size - len, stream)) {
      if (len > 0) // If we read some characters so far, return them
        return line;

      free(line);
      return nullptr;
    }

    len += strlen(line + len);

    if (len > 0 && line[len - 1] == '\n') {
      line[len - 1] = 0;
      return line;
    }

    size *= 2;
    char* tmp = realloc(line, size);
    if (!tmp) {
      free(line);
      fprintf(stderr, "\nError while reallocating the buffer, exiting!");
      return nullptr;
    }
    line = tmp;
  }
}

// Process the string 'line' by counting every occurence of 'word' and
// replacing it by 'replacement'. Dump the modified string to 'stream'.
void replaceWordsAndPrint(const char* line, const char* word,
                          const char* replacement, size_t* count,
                          FILE* stream) {
  if (!line || !word)
    return;

  for (const char* c = line; *c; ++c) {
    if ((c != line) && (isalnum((unsigned char)*(c-1)))) {
      fprintf(stream, "%c", *c);
      continue;
    }

    const char* cpos = c;
    const char* wpos = word;

    while (*wpos && *cpos == *wpos) {
      ++cpos;
      ++wpos;
    }

    if (!*wpos && (isspace((unsigned char)*cpos) || ispunct((unsigned char)*cpos) || *cpos == '\0')) {
      fprintf(stream, "%s", replacement);
      ++(*count);
      c = cpos - 1;
    } else {
      fprintf(stream, "%c", *c);
    }
  }
}

int main(int argc, char* argv[argc+1]) {
  // Usage: first argument is the searched word/regex, the second word is the
  // replacement. Count all the occurences of the first argument in stdin and
  // replace them with the second argument. The modified text will be dumped
  // to stdout.
  if (argc != 3) {
    fprintf(stderr, "Incorrect number of arguments! You need to provide exactly"
        " two arguments.\n");
    return EXIT_FAILURE;
  }
  char* word = argv[1];
  char* replacement = argv[2];

  fprintf(stderr, "Received arguments: \"%s\", and \"%s\".\n", word, replacement);
 
  size_t count = 0;

  for(;;) {
    char* line = fgetline(stdin);
    if (line) {
      // Process line
      replaceWordsAndPrint(line, word, replacement, &count, stdout);
      fprintf(stdout, "\n");
      free(line);
    } else {
      // Check if we are at the end of stream
      fprintf(stderr, "\nFinished processing the line. Found %zu occurrences"
              " of the word \"%s\".\n", count, word);
      return EXIT_SUCCESS;
    }
  }
}
