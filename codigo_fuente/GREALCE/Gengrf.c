
#include "gengrf.h"

#include <assert.h>

//--------------------------------------------------------------------
#ifdef CVIP_tools


void set_pixel(MyTImage *im,int row,int col, unsigned char color) {
  //assert(im!=NULL);
  //assert(row>=0 && col>=0 && row<num_rows(im) && col<num_cols(im));
  //if (row>=0 && col>=0 && row<num_rows(im) && col<num_cols(im)) {
    im->image_ptr[0]->rptr[row][col]=(unsigned char) color;
    #ifdef JPG
      im->image_ptr[1]->rptr[row][col]=(unsigned char) color;
      im->image_ptr[2]->rptr[row][col]=(unsigned char) color;
    #endif
  //}

}

unsigned char read_pixel(MyTImage *im,int row,int col) {
  //assert(im!=NULL);
  //assert(row>=0 && col>=0 && row<num_rows(im) && col<num_cols(im));
  //if (row>=0 && col>=0 && row<num_rows(im) && col<num_cols(im))
    return ((unsigned char)im->image_ptr[0]->rptr[row][col]);
  //else
  //  return 0;
}

unsigned char random_color(int c_type) {
  if (c_type!=0) //se gerean 255 niveles de grises
    return (unsigned char)(rand()%255);
  else //se genera B&N, es decir, ó 0 ó 255
    return (unsigned char)((rand()%2)*255); 
  //0,RAND_MAX

}
int num_rows(MyTImage *in) {
  int rows;
  rows=getNoOfRows_Image(in);
  return rows;
}
int num_cols(MyTImage *in) {
  int cols;
  cols=getNoOfCols_Image(in);
  return cols;
}
int size_mem_image(MyTImage *in) {
  int mem,rows,cols;
  mem=0;
  rows=num_rows(in);
  cols=num_cols(in);
  //pixeles
  mem+=sizeof(in->image_ptr[0]->rptr[0][0])*rows*cols;
  //puntero a array de pixeles
  mem+=sizeof(in->image_ptr[0]->rptr)*3;
  //punteros a bandas de imagen
  mem+=sizeof(in->image_ptr)*3;
  //puntero a imagen
  mem+=sizeof(in);
  return mem;
}
MyTImage *read_image(const char *filename) {
  MyTImage *im;
  return im=read_Image(filename, 1);
  //FALTA: comprobar que el fichero existe
  return im;
}
int write_image(const char *filename, MyTImage *im) {
  //FILE *aux;
  //aux=stdout;
  //stdout=NULL;
  write_Image(im,filename,CVIP_NO,CVIP_NO,GRF_FORMAT,CVIP_NO);
  //stdout=aux;
  return 0;
}

MyTImage *create_image(int rows,int cols) {
  MyTImage *im;
  im=new_Image(GRF_FORMAT,GRAY_SCALE,BANDS,rows,cols,CVIP_BYTE,REAL);
  return im;
}

MyTImage *duplicate_image(MyTImage *in) {
  MyTImage *out;
  out=duplicate_Image(in);
  return out;
}

int free_image(MyTImage *im) {
  assert(im!=NULL);
  delete_Image(im);
  /* FALTA: comprobación de que no hubo error */
  return 0;
}

unsigned char black_or_white(unsigned char color,int mode) {
	if (mode==2) 
		return color;
	else {
		if (color>128) return 255;
		else return 0;
	}
}

#endif
//-----------------------------------------------------------------------
#ifdef CBuilder

void set_pixel(MyTImage *im,int row,int col, unsigned char color) {
  //assert(im!=NULL);
  //assert(row>=0 && col>=0 && row<num_rows(im) && col<num_cols(im));
  //if (row>=0 && col>=0 && row<num_rows(im) && col<num_cols(im))
    im->Bitmap->Canvas->Pixels[row][col]=color*0x00FFFFFF;
}
unsigned char read_pixel(MyTImage *im,int row, int char col) {
  //assert(im!=NULL);
  //assert(row>=0 && col>=0 && row<num_rows(im) && col<num_cols(im));
  //if (row>=0 && col>=0 && row<num_rows(im) && col<num_cols(im))
  return (unsigned char)(im->Bitmap->Canvas->Pixels[row][col])/0x00FFFFFF;
  //else return 0;

}
unsigned char random_color(int mode) {
  if (mode==2) {
    return ((unsigned char)(rand()%255));
  else
    return (unsigned char)((rand()%2)); // Black or White
  //0,RAND_MAX
}
int num_rows(MyTImage *in) {
  int rows;
  rows=in->Height;
  return rows;
}
int num_cols(MyTImage *in) {
  int cols;
  cols=in->Width;
  return cols;
}
int size_mem_image(MyTImage *in) {
  int mem,rows,cols;
  mem=0;
  return mem;
}
MyTImage *read_image(const char *filename) {
  MyTImage *im;
  im=new MyTImage;
  im->LoadFromFile(filename);
  return im;
}
int write_image(const char *filename, MyTImage *im) {
  im->SaveToFile(filename);
  delete im;
  return 0;
}
MyTImage *create_image(int rows,int cols) {
  MyTImage *im;
  im=new MyTImage;
  im->Bitmap->Width=cols;
  im->Bitmap->Height=rows;
  return im;
}
MyTImage *duplicate_image(MyTImage *in) {
  MyTImage *out;
  out=new MyTImage;
  out->Bitmap=in->Bitmap;
  //el operador '=' sobre Bitmap, en primer lugar copia
  // los punteros, pero más adelante al intentar escribir
  // sobre out, se duplica el contenido del objeto (segun
  // la ayuda del borland).
  return out;
}
int free_image(MyTImage *im) {
  assert(im!=NULL);
  delete im;
  return 0;
}
#endif