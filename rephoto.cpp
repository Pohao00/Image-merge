#include<stdio.h>
#include<stdlib.h>
#include<math.h>

typedef long INT32;
typedef unsigned short int INT16;
typedef unsigned char U_CHAR;

#define UCH(x)	((int) (x))
#define GET_2B(array,offset)  ((INT16) UCH(array[offset]) + \
(((INT16) UCH(array[offset+1])) << 8))
#define GET_4B(array,offset)  ((INT32) UCH(array[offset]) + \
(((INT32) UCH(array[offset+1])) << 8) + \
(((INT32) UCH(array[offset+2])) << 16) + \
(((INT32) UCH(array[offset+3])) << 24))
#define FREAD(file,buf,sizeofbuf)  \
((size_t) fread((void *) (buf), (size_t) 1, (size_t) (sizeofbuf), (file)))


int main()
{
 FILE *input_file = 0 ;
 FILE *output_file = 0 ;
 
 U_CHAR bmpfileheader[14] = { 0 } ;
 U_CHAR bmpinfoheader[40] = { 0 } ;

 INT32 FileSize = 0 ;
 INT32 bfOffBits =0 ;
 INT32 headerSize =0 ;
 INT32 biWidth = 0 ;		
 INT32 biHeight = 0 ;
 INT16 biPlanes = 0 ;
 INT16 BitCount = 0 ;
 INT32 biCompression = 0 ;
 INT32 biImageSize = 0;
 INT32 biXPelsPerMeter = 0 ,biYPelsPerMeter = 0 ;
 INT32 biClrUsed = 0 ;
 INT32 biClrImp = 0 ;

 U_CHAR *data1,*data2,*data3,*data4,*data5,*data6, *new_data, color_table[1024];
 int i, j, k, biWidth4,n;

   /* 開啟檔案 */
 if( ( input_file = fopen("new36.bmp","rb") ) == NULL ){
  fprintf(stderr,"File can't open.\n");
  exit(0);
}


FREAD(input_file,bmpfileheader,14);
FREAD(input_file,bmpinfoheader,40);

   if (GET_2B(bmpfileheader,0) == 0x4D42) /* 'BM' */
fprintf(stdout,"BMP file.\n");
else{
  fprintf(stdout,"Not bmp file.\n");
  exit(0);
}

FileSize           =   GET_4B(bmpfileheader,2);
bfOffBits         =   GET_4B(bmpfileheader,10);
headerSize      =   GET_4B(bmpinfoheader,0);
biWidth           =   GET_4B(bmpinfoheader,4);
biHeight          =   GET_4B(bmpinfoheader,8);
biPlanes          =   GET_2B(bmpinfoheader,12);
BitCount          =   GET_2B(bmpinfoheader,14);
biCompression   =   GET_4B(bmpinfoheader,16);
biImageSize      =   GET_4B(bmpinfoheader,20);
biXPelsPerMeter =   GET_4B(bmpinfoheader,24);
biYPelsPerMeter =   GET_4B(bmpinfoheader,28);
biClrUsed         =   GET_4B(bmpinfoheader,32);
biClrImp          =   GET_4B(bmpinfoheader,36);

printf("FileSize = %ld \n"
	"DataOffset = %ld \n"
 "HeaderSize = %ld \n"
 "Width = %ld \n"
 "Height = %ld \n"
 "Planes = %d \n"
 "BitCount = %d \n"
 "Compression = %ld \n"
 "ImageSize = %ld \n"
 "XpixelsPerM = %ld \n"
 "YpixelsPerM = %ld \n"
 "ColorsUsed = %ld \n"
 "ColorsImportant = %ld \n",FileSize,bfOffBits,headerSize,biWidth,biHeight,biPlanes,
 BitCount,biCompression,biImageSize,biXPelsPerMeter,biYPelsPerMeter,biClrUsed,biClrImp);

if (BitCount != 24) {
  fprintf(stderr,"Not a 24-bit file.\n");
  fclose (input_file);
  exit(0);
}

   biWidth4 = ((biWidth*3+3)/4 *4); // For 24-bit color file

   //
   data1 = (U_CHAR *)malloc( biWidth4*biHeight );
   if (data1 == NULL) {
    fprintf(stderr,"Insufficient memory.\n");
    fclose (input_file);
    exit(0);
  }
  
   //
  fseek(input_file,bfOffBits,SEEK_SET);
  FREAD(input_file,data1,biWidth4*biHeight);
   // 
  fclose (input_file);

  if( ( input_file = fopen("new45.bmp","rb") ) == NULL ){
    fprintf(stderr,"File can't open.\n");
    exit(0);
  }

  FREAD(input_file,bmpfileheader,14);
  FREAD(input_file,bmpinfoheader,40);

   if (GET_2B(bmpfileheader,0) == 0x4D42) /* 'BM' */
  fprintf(stdout,"BMP file.\n");
  else{
    fprintf(stdout,"Not bmp file.\n");
    exit(0);
  }

   //
  data2 = (U_CHAR *)malloc( biWidth4*biHeight );
  if (data2 == NULL) {
    fprintf(stderr,"Insufficient memory.\n");
    fclose (input_file);
    exit(0);
  }
  
   //
  fseek(input_file,bfOffBits,SEEK_SET);
  FREAD(input_file,data2,biWidth4*biHeight);
   // 
  fclose (input_file);

  data3 = (U_CHAR *)malloc( biWidth4*biHeight );
  data4 = (U_CHAR *)malloc( biWidth4*biHeight );
  data5 = (U_CHAR *)malloc( biWidth4*biHeight );
  data6 = (U_CHAR *)malloc( biWidth4*biHeight );

   // Process the file
  for (i=0; i < biHeight; i++)
  {
   k = i* biWidth4;
   for (j=0; j < biWidth; j++)
   {
     for( n=0;n<3;n++)
     {
       if (k % 2 == 0)
       {
        data3[k+n] = 255;
        data5[k+n] = 255;
      }
      else
      {
        data3[k+n] = data1[k];
        data5[k+n] = data2[k];
      }
      if (k % 2 != 0)
      {
        data4[k+n] = 255;
        data6[k+n] = 255;
      }
      else
      {
        data4[k+n] = data1[k];
        data6[k+n] = data2[k];
      }
    }
    k = k + 3;
  }
}


for (i=0; i < biHeight; i++)
{
 k = i* biWidth4;
 for (j=0; j < biWidth; j++)
 {
  for( n=0;n<3;n++)
  {
    data1[k+n]=data4[k+n]*data5[k+n];
    data2[k+n]=data3[k+n]*data6[k+n];
  }
  k = k + 3;
}
}

for (i = 0; i < biHeight; i++)
{
  k = i* biWidth4;
  for (j = 0; j < biWidth; j++)
  {
   for( n=0;n<3;n++)
   {
     data4[biWidth4*biHeight-k+n-1]=data1[k+n];
   }
   k = k + 3;
 }
}

   //
   /* 開啟新檔案 */
if( ( output_file = fopen("new11.bmp","wb") ) == NULL ){
  fprintf(stderr,"Output file can't open.\n");
  exit(0);
}

fwrite(bmpfileheader, sizeof(bmpfileheader), 1, output_file);
fwrite(bmpinfoheader, sizeof(bmpinfoheader), 1, output_file);

fwrite(data4, biWidth4*biHeight, 1, output_file);

fclose (output_file);

if( ( output_file = fopen("new22.bmp","wb") ) == NULL ){
  fprintf(stderr,"Output file can't open.\n");
  exit(0);
}

fwrite(bmpfileheader, sizeof(bmpfileheader), 1, output_file);
fwrite(bmpinfoheader, sizeof(bmpinfoheader), 1, output_file);

fwrite(data2, biWidth4*biHeight, 1, output_file);

fclose (output_file);

return 0;
}
