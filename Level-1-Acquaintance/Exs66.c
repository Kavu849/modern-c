#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

enum { buf_max = 32, };

int main(int argc, char* argv[argc+1]) {
  int ret = EXIT_FAILURE;
  char buffer[buf_max] = { };
  bool printLineNumbers = false;

  if (argc > 1 && (strcmp(argv[1], "-n") == 0))
      printLineNumbers = true;

  if (argc == 1 || (argc == 2 && printLineNumbers)) {
    FILE* instream = stdin;
    if (instream) {
      size_t line = 1;
      while (fgets(buffer, buf_max, instream)) {
        size_t len = strlen(buffer);
        bool lineEnded = (len > 0 && buffer[len - 1] == '\n') ? true : false;
        if (printLineNumbers && lineEnded) {
          printf("%zu ", line);
          ++line;
        }
        fputs(buffer, stdout);
      }
      fclose(instream);
      ret = EXIT_SUCCESS;
    } else {
      printf("Could not read from stdin\n");
      perror(0);
      errno = 0;
    }
    return ret;
  }

  for (int i = 1; i < argc; ++i) {        // Process args
    if (i == 1 && printLineNumbers)
      continue;

    FILE* instream = fopen(argv[i], "r"); // as filenames
    if (instream) {
      size_t line = 1;
      bool pendingNewline = false;

      while (fgets(buffer, buf_max, instream)) {
        size_t len = strlen(buffer);
        bool lineEnded = (len > 0 && buffer[len - 1] == '\n') ? true : false;
        if (printLineNumbers && (lineEnded || !pendingNewline)) {
          printf("%zu ", line);
          ++line;
        }
        fputs(buffer, stdout);
        pendingNewline = !lineEnded;
      }
      fclose(instream);
      ret = EXIT_SUCCESS;
    } else {
      /* Provides some error diagnostic */
      fprintf(stderr, "Could not open %s: ", argv[i]);
      perror(0);
      errno = 0;  // Resets the error code
    }
  }
  return ret;
}
