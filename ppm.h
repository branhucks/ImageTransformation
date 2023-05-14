#ifndef PPM_H
#define PPM_H


typedef struct{
  unsigned char r, g, b;
} PPMPixel;

typedef struct{
  int cols; // width
  int rows; // height
  int maxc; // max color value (e.g., 255)
  char *magic; // magic number (e.g., 'P6')
  PPMPixel *data;
} PPM;

// function protoypes (interface)
void ppm_header(FILE *in, int *cols, int *rows, int *maxc);
PPM *ppm_alloc(int cols, int rows, int maxc);
void writePPM(char* outfile, PPM* img);
void grayscale(PPM* simg, PPM* dimg);
void rotate(PPM* simg, PPM* dimg);
void shrink(PPM* simg, PPM* dimg);
void invert(PPM* simg, PPM* dimg);

#endif
