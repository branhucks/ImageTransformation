/*
Brandon Hucks
12/06/2022
Program Description: This program will read in a .ppm file specified by the
command-line argument following the name of the executable/binary file.
The program will then perform one of four image transformations: grayscale,
invert, rotate, or shrink. The program will then save the new transformed
image to a new .ppm file adding _theexecutedtransformation to the end of the
original filename.
*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#include "ppm.h"

int main(int argc, char *argv[])
{
  int rows, cols, maxc;
  char *filename;
  char *destFile;
  char *trans;
  FILE *fin = NULL;
  PPM *simg = NULL, *dimg = NULL;

  // Verify that specified parameters match expectation
  if (argc != 3)
  {
    fprintf(stdout, "Must include filename and image transformation\n");
    fprintf(stdout, "Example: ./main oneTiger.ppm g\n");
    exit(1);
  }
  filename = argv[1];
  trans = argv[2];
  char *valTrans = "nNsSrRgG";
  if (strchr(valTrans, *trans) == NULL)
  {
    fprintf(stdout, "Invalid transformation\n");
    fprintf(stdout, "Valid transformations: g G, r R, s S, n N\n");
    exit(1);
  }

  // Read input file
  if (!(fin = fopen(filename, "rb")))
  {
    fprintf(stdout, "error: could not open file: %s\n", filename);
    exit(1);
  }
  ppm_header(fin, &cols, &rows, &maxc);
  fprintf(stdout, "cols: %d, rows: %d, maxc: %d\n", cols, rows, maxc);
  if (!(simg = ppm_alloc(cols, rows, maxc)))
  {
    fprintf(stdout, "error: got NULL image pointer\n");
    exit(1);
  }
  simg->data = (PPMPixel *)malloc(simg->cols * simg->rows * sizeof(PPMPixel));
  // read in r, g, b data into simg
  if (fread(simg->data, 3 * simg->rows, simg->cols, fin) != simg->cols)
  {
    fprintf(stdout, "Error reading in r, g, b data\n");
    exit(1);
  }
  fclose(fin);

  // Allocating
  if (!(dimg = ppm_alloc(cols, rows, maxc)))
  {
    fprintf(stdout, "error: got NULL image pointer\n");
    exit(1);
  }
  dimg->data = (PPMPixel *)malloc(cols * rows * sizeof(PPMPixel));

  // Cases for each transformation
  switch (*trans)
  {
  case 'g':
  case 'G':
    grayscale(simg, dimg);
    filename[strlen(filename) - 4] = '\0';
    destFile = strcat(filename, "_grayscale.ppm");
    break;
  case 'n':
  case 'N':
    invert(simg, dimg);
    filename[strlen(filename) - 4] = '\0';
    destFile = strcat(filename, "_negative.ppm");
    break;
  case 's':
  case 'S':
    shrink(simg, dimg);
    filename[strlen(filename) - 4] = '\0';
    destFile = strcat(filename, "_halfsize.ppm");
    break;
  case 'r':
  case 'R':
    rotate(simg, dimg);
    filename[strlen(filename) - 4] = '\0';
    destFile = strcat(filename, "_rotate.ppm");
    break;
  }

  // Write converted to destination file
  writePPM(destFile, dimg);

  // Freeing allocated memory for images
  free(simg);
  free(dimg);
}
