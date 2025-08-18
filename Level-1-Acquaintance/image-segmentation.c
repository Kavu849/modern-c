#include <stdio.h>
#include <stdlib.h>
#include <MagickWand/MagickWand.h>
#include <math.h>

typedef struct RegionStats RegionStats;
struct RegionStats {
  size_t NumOfPixels;
  size_t SumOfVals;
};

// Definitions of union-find related functions
size_t Find(size_t ind, size_t UnionFind[]) {
  while (UnionFind[ind] != SIZE_MAX) {
    ind = UnionFind[ind];
  }
  return ind;
}

void FindReplace(size_t ind, size_t val, size_t UnionFind[]) {
  while (UnionFind[ind] != SIZE_MAX) {
    size_t tmp = UnionFind[ind];
    UnionFind[ind] = val;
    ind = tmp;
  }
}

size_t FindCompress(size_t ind, size_t UnionFind[]) {
  size_t root = Find(ind, UnionFind);
  FindReplace(ind, root, UnionFind);
  return root;
}

void Union(size_t ind1, size_t ind2, size_t UnionFind[]) {
  size_t root1 = FindCompress(ind1, UnionFind);
  size_t root2 = Find(ind2, UnionFind);
  if (root1 != root2) {
    FindReplace(ind2, root1, UnionFind);
    UnionFind[root2] = root1;
  }
}

// Function that computes computes the statistic for two regions and returns
// true if we should merge them, or false otherwise.
bool ShouldMerge(size_t i, size_t j, size_t k, size_t l, size_t width, size_t UnionFind[], RegionStats Stats[]) {
  size_t ind1 = i * width + j;
  size_t ind2 = k * width + l;

  size_t root1 = Find(ind1, UnionFind);
  size_t root2 = Find(ind2, UnionFind);
  
  // If the roots are the same, don't merge (becasue already merged)
  if (root1 == root2)
    return false;

  double mean1 = Stats[root1].SumOfVals / Stats[root1].NumOfPixels;
  double mean2 = Stats[root2].SumOfVals / Stats[root2].NumOfPixels;
  
  if (fabs(mean1 - mean2) < 5)
    return true;
  else
    return false;
}

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
  unsigned char Pixels[height][width]; 
  MagickBooleanType res =  MagickExportImagePixels(wand, 0, 0, width, height, "R", CharPixel, Pixels);
  if (res == MagickFalse)
    ThrowWandException(wand);
  for (size_t i = 0; i < 100; ++i)
    printf("[%zu, %zu] = %u, ", i, i, Pixels[i][i]);

  // Define the array of statistics
  RegionStats Stats[height * width];

  // Define the Union Find data structure, and fill the Stats array
  size_t UnionFind[height * width];
  for (size_t i = 0; i < height; ++i) {
    for (size_t j = 0; j < width; ++j) {
      UnionFind[i * width + j] = SIZE_MAX;
      Stats[i * width + j].NumOfPixels = 1;
      Stats[i * width + j].SumOfVals = Pixels[i][j];
    }
  }


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

