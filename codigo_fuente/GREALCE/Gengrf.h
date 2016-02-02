#include "defs.h" //esto define CVIP_tools ó CBuilder

#ifdef CVIP_tools
#include <CVIPtoolkit.h>
#include <CVIPimage.h>
#include <CVIPconvert.h>
typedef Image MyTImage;
#endif

#ifdef CBuilder
#include <vcl.h>
typedef TPicture MyTImage;
#endif

void set_pixel(MyTImage *im,int row,int col, unsigned char color);
unsigned char read_pixel(MyTImage *im,int row,int col);
unsigned char random_color(int mode);
int num_rows(MyTImage *in);
int num_cols(MyTImage *in);
int size_mem_image(MyTImage *in);
MyTImage *read_image(const char *filename);
int write_image(const char *filename, MyTImage *im);
MyTImage *create_image(int rows,int cols);
MyTImage *duplicate_image(MyTImage *in);
int free_image(MyTImage *im);
unsigned char black_or_white(unsigned char color, int mode);