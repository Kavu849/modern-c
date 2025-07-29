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

  int startArg = 1;
  if (printLineNumbers)
    ++startArg;

  if (argc == 1 || (argc == 2 && printLineNumbers)) {
    size_t line = 1;
    bool previousLinePartial = false;

    while (fgets(buffer, buf_max, stdin)) {
      size_t len = strlen(buffer);
      bool lineEnded = (len > 0 && buffer[len - 1] == '\n') ? true : false;
      
      if (printLineNumbers && !previousLinePartial) {
        printf("%zu ", line);
      }

      fputs(buffer, stdout);

      if (lineEnded) {
        ++line;
        previousLinePartial = false;
      } else {
        previousLinePartial = true;
      }
    }
    ret = EXIT_SUCCESS;
    return ret;
  }

  for (int i = startArg; i < argc; ++i) {        // Process args
    FILE* instream = fopen(argv[i], "r"); // as filenames
    if (instream) {
      size_t line = 1;
      bool previousLinePartial = false;

      while (fgets(buffer, buf_max, instream)) {
        size_t len = strlen(buffer);
        bool lineEnded = (len > 0 && buffer[len - 1] == '\n') ? true : false;
        
        if (printLineNumbers && !previousLinePartial) {
          printf("%zu ", line);
        }
        
        fputs(buffer, stdout);
        
        if (lineEnded) {
          ++line;
          previousLinePartial = false;
        } else {
          previousLinePartial = true;
        }
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
