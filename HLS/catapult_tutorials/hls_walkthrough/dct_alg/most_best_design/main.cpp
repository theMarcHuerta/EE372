//
// Copyright 2003-2015 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
// 
#include "jpeg.h"
#include <ccs_jpgfile.h>
#include <ccs_bmpfile.h>
#include <mc_scverify.h>

#include <cstring>
#include <iostream>

using std::cout;
using std::endl;

void jpgencoder(const char * bmpfilename, const char * jpgfilename)
{
  cout << " Reading BMP file " << bmpfilename << endl;

  ccs_bmpfile * bitmap = new ccs_bmpfile(bmpfilename);

  cout << " Compressing to JPG file " << jpgfilename << endl;

  // Construct a Jpeg output object
  ccs_jpgfile fp(jpgfilename);

  assert(bitmap != NULL);

  // rows and columns must be divisible by 8, so make bigger if necessary
  if (bitmap->rows() % 8) {
    printf("  ERROR: number of rows (image width) must be divisible by 8. please resize original bitmap image\n");
    exit(-1);
  }
  if (bitmap->columns() % 8) {
    printf("  ERROR: number of columns (image height) must be divisible by 8. please resize original bitmap image\n");
    exit(-1);
  }

  unsigned int img_rows = bitmap->rows();
  unsigned int img_columns = bitmap->columns();

  rgb_t         rgb[64];           // 8x8 RGB block (R:0, G:1, B:2)
  codes_t       codes[64];         // array of Huffman codes

  ac_channel<rgb_t>   rgbstream;   // streamed array of RGB pixels 
  ac_channel<codes_t> hufstream;   // streamed array of Huffman codes
  ac_channel<uint2>   blocktype; 
  
  
  // write out all the header info for the jpeg
  fp.writepreamble(bitmap->rows(), bitmap->columns());

  // for every 8x8 pixel block in the bitmap image
  for (unsigned i = 0; i < img_rows; i += 8) {
    for (unsigned j = 0; j < img_columns; j += 8) {

      // extract 8x8 RGB block from bitmap array
      for (unsigned int x=0; x<8; x++) {
        for (unsigned int y=0; y<8; y++) {
          rgb[x*8+y].r = bitmap->red(i+x, j+y);
          rgb[x*8+y].g = bitmap->green(i+x, j+y);
          rgb[x*8+y].b = bitmap->blue(i+x, j+y);
        }
      }

      // process Y, Cb and Cr blocks, one after the other
      for (uint2 type = 0; type <= 2; type++) {
        
        for (int i=0; i<64; i++) 
          rgbstream.write(rgb[i]);
        
        blocktype.write(type);
        // THE HLS CODE AND BLOCK RIGTH HERE LOCATED IN JPEG
        pixelpipe(blocktype, rgbstream, hufstream);

        for (int i=0; i<64; i++) 
          codes[i] = hufstream.read();
        
        // write out the bitstream
        for (int k=0; k<64; k++)
          fp.write(codes[k].size, codes[k].code);
      }
    }
    printf("Finished row %3d out of %3d...", i, img_rows); cout << endl;
  }

  // write out trailing info for the jpeg
  fp.writepostscript();

  cout << " ...Done" << endl << endl;

}


CCS_MAIN(int argc, const char *argv[])
{
  char *output;
  
  switch (argc) {
    case 3:
      jpgencoder(argv[1], argv[2]);
      break;
    case 2:
      output = (char *)malloc(strlen(argv[1])+4);
      strcpy(output, argv[1]);
      strcat(output, ".jpg");
      jpgencoder(argv[1], output);
      break;
    default:
      jpgencoder("sample.bmp", "sample.bmp.jpg");
      break;
  }
  CCS_RETURN(0);
}
