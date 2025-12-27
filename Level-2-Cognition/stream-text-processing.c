#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Fill the buffer 'buf' from 'stream' starting at currPos and ending when
// we reach endBuf, which points one byte past buf. Note that currPos must
// point to the first empty element in the buffer. Return the number of read
// characters.
int fillBuff(FILE* stream,  char** currPos, char* endBuf) {
  int ret = 0;
  while (*currPos < endBuf) {
    int val = fgetc(stream);
    if (val == EOF)
      return -1;
    **currPos = val;
    ++(*currPos);
    ++ret;
  }
  return ret;
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
  size_t len = strlen(word);

  printf("Received arguments: \"%s\", and \"%s\".\n", word, replacement);

  // Steps:
  // 1. Create a buffer with sufficiently large size, so that the half of it
  // is greater than the length of 'word'. The buffer will be divided into two
  // parts: safe zone and danger zone, in order to handle problems with the
  // word boundary.
  // 2. Read from stdin until the entire buffer is full. Then, search it for
  // 'word' until we reach the danger zone. For every found 'word', increment
  // the counter and dump the modified text to stdout.
  // 3. Once we reach the danger zone, move the remaining characters to the
  // beginning of the buffer.
  // 4. Go back to step 2.
  
  char buf[256];
  if (256 / 2 < len) {
    fprintf(stderr, "Length of the first argument too large! The maximum length"
        " is 128, and received %ld.\n", len);
    return EXIT_FAILURE;
  }

  char* bufBeg = &buf[0];
  char* endBuf = buf + 256;
  char* currPos = bufBeg;
  for(;;) {
    // Fill the buffer and return the number of read characters
    int read = fillBuff(stdin, &currPos, endBuf);
    // If we are at the EOF, handle it
    // ===== HERE HANDLE EOF =====
    if (read == -1) {
      printf("Reached end of file. Buffer contents:\n");
      for (const char* tmpBuf = bufBeg; tmpBuf != currPos; ++tmpBuf) {
        printf("%c", *tmpBuf);
      }
      break;
    }
    printf("Read %d characters. Buffer contents so far:\n", read);
    for (const char* tmpBuf = bufBeg; tmpBuf != currPos; ++tmpBuf) {
      printf("%c", *tmpBuf);
    }
    currPos = bufBeg;
    
    // Search for 'word', until the danger zone, and return the number of
    // occurrences
    //size_t n = searchBuf(
  }

  return EXIT_SUCCESS;
}
