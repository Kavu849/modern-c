#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TextBlob TextBlob;
struct TextBlob {
  char* Text;
  size_t Len; // Len will not include the \0 terminator
  TextBlob* Next;
  TextBlob* Prev;
};

void SplitText(TextBlob *FirstBlob, size_t Index) {
  if (Index >= FirstBlob->Len)
    return;
  
  // Allocate memory for the new blob
  TextBlob* SecondBlob = malloc(sizeof(TextBlob));
  if (!SecondBlob) {
    perror("malloc failed for SecondBlob!");
    exit(1);
  }
  
  // Allocate memory for the text in the second blob
  size_t SecondLen = FirstBlob->Len - (Index + 1); // include Text[Index] in 
                                                   // the first blob
  SecondBlob->Text = malloc(SecondLen + 1); // extra + 1 for the \0 terminator
  if (!SecondBlob->Text) {
    perror("malloc failed for Text!");
    free(SecondBlob);
    exit(1);
  }

  // Copy elements from the first blob to the second blob
  memcpy(SecondBlob->Text, FirstBlob->Text + Index + 1, SecondLen);
  SecondBlob->Text[SecondLen] = '\0';
  SecondBlob->Len = SecondLen;

  // Allocate memory for the remaining part of first blob
  size_t FirstLen = Index + 1;
  char* TmpText = malloc(FirstLen + 1); // extra +1 for the \0 terminator
  if (!TmpText) {
    perror("malloc failed for TmpText!");
    free(SecondBlob->Text);
    free(SecondBlob);
    exit(1);
  }

  memcpy(TmpText, FirstBlob->Text, FirstLen);
  TmpText[FirstLen] = '\0';
  
  free(FirstBlob->Text);
  FirstBlob->Text = TmpText;
  FirstBlob->Len = FirstLen;

  // Adjust the pointers
  SecondBlob->Prev = FirstBlob;
  SecondBlob->Next = FirstBlob->Next;
  FirstBlob->Next = SecondBlob;
  if (SecondBlob->Next)
    SecondBlob->Next->Prev = SecondBlob;
}

int main(int argc, char* argv[argc + 1]) {
  return 0;
}
