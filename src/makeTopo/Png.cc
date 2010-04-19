#include "png.h"
#include "stdlib.h"
const int PNG_BYTES_TO_CHECK = 4;
const int ERROR = -1;
int WritePNG(char *FName,int H,int W,int BPP, char **Buff)
{
  FILE *fp;
  png_infop info_ptr;
  int bit_depth=BPP;
  long RealRowSize=W/(sizeof(unsigned char)*8);
  int pixel_size = 1;
  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (png_ptr == NULL) {
    exit(0);
  }
  png_bytep *row_pointers = (unsigned char **)png_malloc(png_ptr, H*sizeof(png_bytep));
  for (int i = 0; i < H; i++)
    row_pointers[i] = (unsigned char *)png_malloc(png_ptr, W*pixel_size);
  int Width=(BPP==1)?RealRowSize:W;
  for(int i=0;i<H;i++)
    for(int j=0;j<RealRowSize;j++)
      {
	for(int k=0;k<sizeof(unsigned char);k++)
	  {
	   int  V;
	   if(BPP==1)
	     {
	       V=((Buff[i][j] >> k*8) & 0xFF);
	       row_pointers[i][j*sizeof(unsigned char)+k]=V;
	     }
	   else
	     row_pointers[i][j]=Buff[i][j];

	  }
      }
  fp = fopen(FName, "wb");
  if (fp == NULL)
    return ERROR;
  fprintf(stderr,"\nCreating File %s",FName);
  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (png_ptr == NULL) {
    fclose(fp);
    return ERROR;
  }

  info_ptr = png_create_info_struct(png_ptr);
  if (info_ptr == NULL) {
    fclose(fp);
    png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
    return ERROR;
  }

  if (setjmp(png_jmpbuf(png_ptr))) {
    png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
    fclose(fp);
    return ERROR;
  }

  png_init_io(png_ptr, fp);
  png_set_IHDR(png_ptr, info_ptr, H,W, bit_depth, PNG_COLOR_TYPE_GRAY,
	       PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
  png_set_rows(png_ptr, info_ptr, row_pointers);
  png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, png_voidp_NULL);

  return 0;
  
}
