#include <stdio.h>
#include <stdlib.h>
#include <MagickWand/MagickWand.h>

int main(int argc, char* argv[argc+1]) {
#define ThrowWandException(wand) \
{ \
  char \
    *description; \
 \
  ExceptionType \
    severity; \
 \
  description=MagickGetException(wand,&severity); \
  (void) fprintf(stderr,"%s %s %lu %s\n",GetMagickModule(),description); \
  description=(char *) MagickRelinquishMemory(description); \
  exit(-1); \
}

  if (argc != 3) {
      (void) fprintf(stdout,"Usage: %s image thumbnail\n",argv[0]);
      exit(0);
  }
  /*
    Read an image.
  */
  MagickWandGenesis();
  MagickWand* wand = NewMagickWand();
  MagickBooleanType status = MagickReadImage(wand, argv[1]);
  if (status == MagickFalse)
    ThrowWandException(wand);

  /*
    Turn the images into an array of unsigned chars
  */
  MagickSetImageColorspace(wand, GRAYColorspace);
  MagickSetImageDepth(wand, 8);
  size_t width = MagickGetImageWidth(wand);
  size_t height = MagickGetImageHeight(wand);
  printf("Width: %zu, heigth: %zu\n", width, height);
  unsigned char* pixels[height][width]; 
  MagickBooleanType res =  MagickExportImagePixels(wand, 0, 0, width, height, "R", CharPixel, pixels);
  if (res == MagickFalse)
    ThrowWandException(wand);

  /*
    Write the image then destroy it.
  */
  status = MagickWriteImages(wand ,argv[2], MagickTrue);
  if (status == MagickFalse)
    ThrowWandException(wand);
  wand = DestroyMagickWand(wand);
  MagickWandTerminus();
  return(0);
}

