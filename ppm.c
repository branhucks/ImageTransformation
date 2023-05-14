/*
Brandon Hucks
12/06/2022
*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#include "ppm.h"

PPM *ppm_alloc(int cols, int rows, int maxc)
{
  PPM *img = NULL;
  if (!(img = (PPM *)malloc(sizeof(PPM))))
    fprintf(stderr, "warning: out of memory\n");
  img->cols = cols;
  img->rows = rows;
  img->maxc = maxc;
  img->magic = (char *)malloc(3 * sizeof(char));
  img->data = NULL;
  // ugly hack: hardcoding 'P6' into image magic number
  strcpy(img->magic, "P6");
  img->data = (PPMPixel *)malloc(img->rows * img->cols * sizeof(PPMPixel));
  return (img);
}

// function implementation
void ppm_header(FILE *in, int *cols, int *rows, int *maxc)
{
  char c;
  // init rows, cols, maxc
  *rows = *cols = *maxc = 0;
  if (!in)
  {
    fprintf(stderr, "null file pointer\n");
    exit(1);
  }
  // read magic number
  if ((c = fgetc(in)) == 'P' && (c = fgetc(in) == '6'))
  {
    // handle comments, if many
    c = fgetc(in);
    if ((c = fgetc(in)) == '#')
      do
      {
        c = fgetc(in);
      } while (c != '\n');
    else
      ungetc(c, in);
    // read in width height
    if (fscanf(in, "%d %d", cols, rows) < 2)
    {
      fprintf(stderr, "couldn't read width height\n");
      exit(1);
    }
    // handle comments, if many
    c = fgetc(in);
    if ((c = fgetc(in)) == '#')
      do
      {
        c = fgetc(in);
      } while (c != '\n');
    else
      ungetc(c, in);
    // read in maxc
    fscanf(in, "%d ", maxc);
    // eat white space
  }
  else
  {
    fprintf(stderr, "unknown image magic number\n");
    exit(1);
  }
}

// Writing PPM
void writePPM(char *outfile, PPM *img)
{
  FILE *fout = NULL;
  if (!(fout = fopen(outfile, "wb")))
  {
    fprintf(stdout, "error: could not open file: %s\n", outfile);
    exit(1);
  }
  fprintf(fout, "P6\n");
  fprintf(fout, "%d %d\n", img->cols, img->rows);
  fprintf(fout, "%d\n", img->maxc);
  // read in r, g, b data into simg
  fwrite(img->data, 3 * img->rows, img->cols, fout);
  fclose(fout);
}

// Converting image to grayscale
void grayscale(PPM *simg, PPM *dimg)
{
  fprintf(stdout, "Converting to grayscale\n");
  int i;
  if (simg)
  {
    dimg->cols = simg->cols;
    dimg->rows = simg->rows;
    dimg->maxc = simg->maxc;
    for (i = 0; i < simg->cols * simg->rows; i++)
    {
      dimg->data[i].r = ((simg->data[i].r + simg->data[i].g + simg->data[i].b) / 3);
      dimg->data[i].g = dimg->data[i].r;
      dimg->data[i].b = dimg->data[i].r;
    }
  }
}

// Rotating image 90 degrees clockwise
void rotate(PPM *simg, PPM *dimg)
{
  fprintf(stdout, "Rotating image 90 degrees clockwise\n");
  int i, j;
  int new = 0;
  if (simg)
  {
    dimg->cols = simg->rows;
    dimg->rows = simg->cols;
    dimg->maxc = simg->maxc;
    for (i = ((simg->cols * simg->rows) - (simg->cols - 1));
         i < simg->cols * simg->rows + 1; i++)
    {
      for (j = 0; j < simg->rows; j++)
      {
        dimg->data[new] = simg->data[i - (j * simg->cols)];
        new ++;
      }
    }
  }
}

// Shrinking image to half its original size
void shrink(PPM *simg, PPM *dimg)
{
  fprintf(stdout, "Shrinking image to halfsize\n");
  int i, newr, newg, newb;
  int destI = 0;
  dimg->cols = simg->cols / 2;
  dimg->rows = simg->rows / 2;
  dimg->maxc = simg->maxc;
  for (i = 0; i < simg->rows * simg->cols; i = i + 2)
  {
    // Skip row if at beginning of next row
    if (i > 0 && i % simg->cols == 0)
    {
      i = i + simg->cols;
    }
    // Skip first column if odd number of columns
    if (simg->cols % 2 != 0 && i % simg->cols == 0)
    {
      i = i + 1;
    }
    // Averaging r
    newr = simg->data[i].r;
    newr = newr + simg->data[i + 1].r;
    newr = newr + simg->data[i + simg->cols].r;
    newr = newr + simg->data[i + simg->cols + 1].r;
    newr = newr / 4;
    // Averaging g
    newg = simg->data[i].g;
    newg = newg + simg->data[i + 1].g;
    newg = newg + simg->data[i + simg->cols].g;
    newg = newg + simg->data[i + simg->cols + 1].g;
    newg = newg / 4;
    // Averaging b
    newb = simg->data[i].b;
    newb = newb + simg->data[i + 1].b;
    newb = newb + simg->data[i + simg->cols].b;
    newb = newb + simg->data[i + simg->cols + 1].b;
    newb = newb / 4;
    dimg->data[destI].r = newr;
    dimg->data[destI].g = newg;
    dimg->data[destI].b = newb;
    destI++;
    if (destI > dimg->cols * dimg->rows)
    {
      break;
    }
  }
}

// Inverting the colors
void invert(PPM *simg, PPM *dimg)
{
  fprintf(stdout, "Inverting colors of image\n");
  int i;
  if (simg)
  {
    dimg->cols = simg->cols;
    dimg->rows = simg->rows;
    dimg->maxc = simg->maxc;
    for (i = 0; i < simg->cols * simg->rows; i++)
    {
      dimg->data[i].r = 255 - simg->data[i].r;
      dimg->data[i].g = 255 - simg->data[i].g;
      dimg->data[i].b = 255 - simg->data[i].b;
    }
  }
}
