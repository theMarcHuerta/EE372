#ifndef RENDER_TOP_CPP
#define RENDER_TOP_CPP


#ifdef __SYNTHESIS__
    #define LABEL(x) x:
#else
    #define LABEL(x) {}
#endif

#include "render.h"
#include <mc_scverify.h>

#include "Serializer.h"
#include "Deserializer.h"

#include "InputDoubleBuffer.h"
#include "WeightDoubleBuffer.h"
#include "SystolicArray.h"

#include <ac_int.h>
#include <ac_fixed.h>
#include <ac_math.h>
#include <ac_channel.h>

// File: render.cpp
//
// Description: 
//
// Template Parameters:
//    pType - configures the data type of the input and output data
//    accuType - configures the data type of the internal accumulator variables
//    IMG_W, IMG_H - configures the maximum width/height of the input data
//    K_R, K_C - configures the kernel size (rows and columns)
//    AC_MODE - configures the windowing mode (AC_MIRROR or AC_CLIP)




  ac_channel<rgb_t>   rgbstream;   // streamed array of RGB pixels 


#pragma hls_design top
class render{
public:
    render(){}
    #pragma hls_design interface
    void CCS_BLOCK(run)(ac_channel<PackedInt<INPUT_PRECISION, 4> > &input_serial, 
                        ac_channel<PackedInt<WEIGHT_PRECISION, 4> > &weight_serial, 
                        ac_channel<ODTYPE> &output_serial,
                        ac_channel<uint_16> &paramsIn)
    {
        paramsDeserializer.run(paramsIn, inputDoubleBufferParams, weightDoubleBufferParams, systolicArrayParams, outputSerializerParams);

        inputDoubleBuffer.run(input_serial, input_out, inputDoubleBufferParams);
        weightDoubleBuffer.run(weight_serial, weight_out, weightDoubleBufferParams);
        systolicArray.run(input_out, weight_out, output, systolicArrayParams);

        outputSerializer.run(output, output_serial, outputSerializerParams);   
    }

private:
    ParamsDeserializer paramsDeserializer;
    Serializer<PackedInt<OUTPUT_PRECISION, ARRAY_DIMENSION>, ODTYPE, ARRAY_DIMENSION, ACCUMULATION_BUFFER_SIZE> outputSerializer;
    ac_channel<Params> outputSerializerParams;

    InputDoubleBuffer<INPUT_BUFFER_SIZE, ARRAY_DIMENSION, ARRAY_DIMENSION> inputDoubleBuffer;
    ac_channel<Params> inputDoubleBufferParams;

    WeightDoubleBuffer<WEIGHT_BUFFER_SIZE, ARRAY_DIMENSION, ARRAY_DIMENSION> weightDoubleBuffer;
    ac_channel<Params> weightDoubleBufferParams;
    
    ac_channel<PackedInt<INPUT_PRECISION,ARRAY_DIMENSION> > input_out;
    ac_channel<PackedInt<WEIGHT_PRECISION,ARRAY_DIMENSION> > weight_out;
    ac_channel<PackedInt<OUTPUT_PRECISION,ARRAY_DIMENSION> > output;    

    SystolicArrayWrapper<IDTYPE,WDTYPE,ODTYPE, ARRAY_DIMENSION, ARRAY_DIMENSION> systolicArray;
    ac_channel<Params> systolicArrayParams;
};

#endif


// //
// // Copyright 2003-2015 Mentor Graphics Corporation
// //
// // All Rights Reserved.
// //
// // THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// // MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
// // 
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
