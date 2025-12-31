#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Process text blobs with support added for multibyte strings using the UTF-8
// encoding.

typedef struct TextBlob TextBlob;
struct TextBlob {
  char* Text;
  size_t Len; // Len will not include the \0 terminator
  TextBlob* Next;
  TextBlob* Prev;
};

int GetTerminalRows() {
  struct winsize w;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
    perror("ioctl failed");
    return -1;
  }
  return w.ws_row;
}

TextBlob* CreateBlob(const char* s) {
  if (!s)
    return nullptr;

  TextBlob* Blob = malloc(sizeof(TextBlob));
  if (!Blob) {
    perror("malloc failed for Blob!");
    exit(1);
  }

  size_t Length = strlen(s);
  char* TempText = malloc(Length + 1);
  if (!TempText) {
    perror("malloc failed for TempText!");
    free(Blob);
    exit(1);
  }

  memcpy(TempText, s, Length);
  TempText[Length] = '\0';

  Blob->Text = TempText;
  Blob->Len = Length;
  Blob->Next = nullptr;
  Blob->Prev = nullptr;

  return Blob;
}

void DestroyList(TextBlob* Head) {
  TextBlob* Current = Head;
  TextBlob* NextBlob = Head->Next;
  while (Current) {
    NextBlob = Current->Next;
    free(Current->Text);
    free(Current);
    Current = NextBlob;
  }
}

// Function checking if 'c' is the start of a new character, which can be
// either ASCII or UTF-8 multibyte. Returns 0 if it is a continuation byte,
// i.e., it is a middle byte of a multibyte character.
bool IsStartByte(unsigned char c) {
  // According to ASCII and UTF-8 rules, a char is a continuation byte iff
  // its top two bits are '10'
  return (c & 0xC0) != 0x80;
}

size_t FindSplitIndex(const char* Text, size_t InitialIndex) {
  size_t len = strlen(Text);
  size_t NextCharIndex = InitialIndex + 1;

  if (NextCharIndex >= len)
    return InitialIndex;

  while (Text[NextCharIndex] != '\0' && !IsStartByte(Text[NextCharIndex]))
    ++NextCharIndex;
  
  return NextCharIndex - 1;
}

void SplitText(TextBlob *FirstBlob, size_t Index) {
  if (Index >= FirstBlob->Len)
    return;
  
  size_t SplitIndex = FindSplitIndex(FirstBlob->Text, Index);

  if (SplitIndex >= FirstBlob->Len)
    return;

  // Allocate memory for the new blob
  TextBlob* SecondBlob = malloc(sizeof(TextBlob));
  if (!SecondBlob) {
    perror("malloc failed for SecondBlob!");
    exit(1);
  }
  
  // Make sure that the index starts at the word boundary

  // Allocate memory for the text in the second blob
  size_t SecondLen = FirstBlob->Len - (SplitIndex + 1); // include Text[Index] in 
                                                   // the first blob
  SecondBlob->Text = malloc(SecondLen + 1); // extra + 1 for the \0 terminator
  if (!SecondBlob->Text) {
    perror("malloc failed for Text!");
    free(SecondBlob);
    exit(1);
  }

  // Copy elements from the first blob to the second blob
  memcpy(SecondBlob->Text, FirstBlob->Text + SplitIndex + 1, SecondLen);
  SecondBlob->Text[SecondLen] = '\0';
  SecondBlob->Len = SecondLen;

  // Allocate memory for the remaining part of first blob
  size_t FirstLen = SplitIndex + 1;
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

// This function will merge the Blob it receives with the next one if it exists
void JoinConsecutive(TextBlob* Blob) {
  if (!Blob || !Blob->Next)
    return;

  // Allocate memory for the new Text
  size_t NewLen = Blob->Len + Blob->Next->Len;
  char* NewText = malloc(NewLen + 1);
  if (!NewText) {
    perror("malloc failed for NewText!");
    exit(1);
  }

  memcpy(NewText, Blob->Text, Blob->Len);
  memcpy(NewText + Blob->Len, Blob->Next->Text, Blob->Next->Len);
  NewText[NewLen] = '\0';

  free(Blob->Text);
  free(Blob->Next->Text);

  Blob->Text = NewText;
  Blob->Len = NewLen;
  TextBlob* TmpBlob = Blob->Next;
  if (Blob->Next->Next) {
    Blob->Next->Next->Prev = Blob;
    Blob->Next = Blob->Next->Next;
  } else
    Blob->Next = nullptr;

  free(TmpBlob);
}

// This function splits the given blob at the first newline character and
// returns the next blob, or nullptr if there's no newline character.
TextBlob* SplitAtFirstNewline(TextBlob* Blob) {
  if (!Blob || !Blob->Text)
    return nullptr;

  for (size_t i = 0; i < Blob->Len; ++i) {\
    if (Blob->Text[i] == '\n') {
      SplitText(Blob, i);
      Blob->Text[i] = '\0'; // Remove the newline character from the blob.
                            // That way, each blob represents one line.
      return Blob->Next;
    }
  }
  return nullptr;
}

// Function that processes the entire string s, diving it into blobs, one
// blob per line. It returns the head of the resulting linked list.
TextBlob* ProcessText(const char* s) {
  // Load the entire string into one big blob. Then, split it as long as we can
  // find a new newline character '\n'
  TextBlob* Head = CreateBlob(s);
  TextBlob* Cur = Head;

  while (Cur)
    Cur = SplitAtFirstNewline(Cur);

  return Head;
}

// Print the entire linked list of blobs
void PrintList(TextBlob* Head) {
  size_t i = 0;
  TextBlob* Cur = Head;
  while (Cur) {
    printf("Blob #: %zu, \"%s\"\n", i++, Cur->Text);
    Cur = Cur->Next;
  }
}

// Print the text blobs until the entire text prints, or we reach terminal size
void PrintLimited(TextBlob* Head) {
  size_t i = 0;
  TextBlob* Cur = Head;
  int MaxLines = GetTerminalRows();
  if (MaxLines == -1)
    return;

  while (Cur && i < MaxLines) {
    printf("Blob #: %zu, \"%s\"\n", i++, Cur->Text);
    Cur = Cur->Next;
  }
}

int main(int argc, char* argv[argc + 1]) {
  TextBlob* Head = CreateBlob("Hello, ");
  Head->Next = CreateBlob("world!");
  Head->Next->Prev = Head;

  printf("Original list:\n");
  PrintList(Head);

  printf("Splitting the first blob at index 2:\n");
  SplitText(Head, 2);
  PrintList(Head);
  DestroyList(Head);

  // Test Case 1: Split and Join with intermediate blobs
  printf("\n--- Test Case 1: Splitting and Joining an Intermediate Blob ---\n");
  TextBlob* Head1 = CreateBlob("One");
  Head1->Next = CreateBlob("Middle");
  Head1->Next->Prev = Head1;
  Head1->Next->Next = CreateBlob("Three");
  Head1->Next->Next->Prev = Head1->Next;

  printf("Original list:\n");
  PrintList(Head1);

  // Split the middle blob
  SplitText(Head1->Next, 3);
  printf("\n After splitting 'Middle' at index 3:\n");
  PrintList(Head1);

  // Join the two new blobs
  JoinConsecutive(Head1->Next);
  printf("\nAfter joining the two new blobs:\n");
  PrintList(Head1);
  DestroyList(Head1);

  // Test Case 2: Splitting an empty string
  printf("\n--- Test Case 2: Splitting an Empty String ---\n");
  TextBlob* Head2 = CreateBlob("");
  PrintList(Head2);

  SplitText(Head2, 0);
  printf("\nAfter attempting to split at index 0:\n");
  PrintList(Head2);
  DestroyList(Head2);

  // Test Case 2: Printing the text divided by lines
  printf("\n--- Test Case 3: Printing the text divided by lines ---\n");
  const char* s = "first line\n second line \n third line \n \n two lines later";
  TextBlob* Head3 = ProcessText(s);
  PrintLimited(Head3);
  DestroyList(Head3);
  
  // Multibyte tests
  printf("\n--- Multibyte UTF-8 Split ---\n");
  TextBlob* HeadUTF8_1 = CreateBlob("Hi€");

  printf("Original String: \"Hi€\"\n");
  printf("Attempt to split at index 2 (inside the Euro symbol)...\n");
  SplitText(HeadUTF8_1, 2);
  printf("Result:\n");
  PrintList(HeadUTF8_1);
  DestroyList(HeadUTF8_1);

  TextBlob* HeadUTF8_2 = CreateBlob("Hi€World");
  printf("Original: \"Hi€World\"\n");
  printf("Splitting at index 4 (after the Euro symbol)...\n");
  SplitText(HeadUTF8_2, 4);
  PrintList(HeadUTF8_2);
  DestroyList(HeadUTF8_2);

  return 0;
}
