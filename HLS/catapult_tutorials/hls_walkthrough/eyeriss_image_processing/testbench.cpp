//******************************************************************************
// File: tb_tmp.cpp
// Description
//   Testbench for PE array
//******************************************************************************

#include <iostream>
#include<cstdlib>
#include<fstream>
#include<algorithm>
#include<string>
#include<cstring>

using namespace std;

#include "bmpUtil/bmp_io.hpp"
#include "types.h"
#include "eyeriss.h"
#include "sw_conv.h"

template <int KSIZE>
void error_message()
{
  if (KSIZE == 3) {
    cout << "Usage: design1 [OPTIONS 1] [OPTIONS 2] INPUT_IMAGE_FILENAME" << endl;
    cout << "  [OPTIONS 1] 0 Outline" << endl;
    cout << "              1 Sharpen" << endl;
    cout << "              2 Gaussian blur" << endl;
    cout << "              3 Emboss" << endl;
    cout << endl;
    cout << "  [OPTIONS 2] Striding Factor" << endl;
  } else if (KSIZE == 5) {
    cout << "Usage: design1 [OPTIONS 1] [OPTIONS 2] INPUT_IMAGE_FILENAME" << endl;
    cout << "  [OPTIONS 1] 0 Gaussian Blur" << endl;
    cout << endl;
    cout << "  [OPTIONS 2] Striding Factor" << endl;
  }
  exit(-1);
}

CCS_MAIN (int argc, char *argv[])
{
  // Set up configuration for testing
  enum {
    WIDTH = 1024,           // Max width
    HEIGHT = 1024,          // Max height
    IFMAP = 1,              // Input channel
    OFMAP = 1,              // Output channel
    KSIZE = 3,              // Kernel size
    PAD_SAME = (KSIZE-1)/2,
    PAD_VALID = 0,
    ZERO_PAD = PAD_VALID,
    // constants computed from the above
    IMG_SZ = (WIDTH + 2*ZERO_PAD) * (HEIGHT + 2*ZERO_PAD),
    DW = eyerissHelper<WIDTH,HEIGHT,KSIZE,IFMAP,OFMAP>::dwidthVal,
    CW = eyerissHelper<WIDTH,HEIGHT,KSIZE,IFMAP,OFMAP>::cntwidthVal,
    WR = eyerissHelper<WIDTH,HEIGHT,KSIZE,IFMAP,OFMAP>::weightrowVal,
    WC = eyerissHelper<WIDTH,HEIGHT,KSIZE,IFMAP,OFMAP>::weightcolVal
  };

  typedef ac_int<DW,false> configType;

  unsigned int FILTER_OPTION = 0;
  int tmp_index = 1;
  if (argc == 4) {
    FILTER_OPTION = (KSIZE == 3) ? atol(argv[tmp_index++])%4 : atol(argv[tmp_index++])%1;
  } else {
    error_message<KSIZE>();
  }
  int stride = atol(argv[tmp_index++]);
  string infile_name(argv[tmp_index]);
  string f_name(infile_name);
  size_t lastindex = f_name.find_last_of(".");
  f_name = f_name.substr(0,lastindex);

  unsigned char **rarray = new unsigned char *[3];
  for (int i=0; i<3; i++) {
    rarray[i] = new unsigned char [IMG_SZ]();
  }
  unsigned long int width;
  long int height;

  cout << "Reading image file: '" << infile_name << "'" << endl;
  bmp_read((char *)infile_name.c_str(), &width, &height, &rarray[0], &rarray[1], &rarray[2]);
  cout << "Image Size (WxH): " << width << " x " << height << endl;
  cout << "Padding used: " << ZERO_PAD << endl;
  int pad_width = width+2*ZERO_PAD;
  int pad_height = height+2*ZERO_PAD;
  if (pad_width > WIDTH) {
    cerr << "Error: Only widths (including the padding) less than or equal to 1024 are supported" << endl;
    exit(-1);
  }

  typedef pxlType::rt_T<pxlType>::mult T;
  T::rt_unary::set<KSIZE+1>::sum macResult;
  cout<<  "Striding factor: " << stride << endl;
  cout << "Adder bit width: " << macResult.width << endl;
  cout << "Main Cnt width: " << CW << endl;
  cout << "Weights Row: " << WR << endl;
  cout << "Weights Col: " << WC << endl;

  double **arr = new double*[IFMAP]; // feature input array for software implementation
  for (int i=0; i<3; i++) {
    arr[i] = new double [IMG_SZ];
  }
  unsigned char **result_h = new unsigned char *[OFMAP]; // hardware result
  for (int i=0; i<OFMAP; i++) {
    result_h[i] = new unsigned char [IMG_SZ]();
  }
  unsigned char **result_s = new unsigned char *[OFMAP]; // software reference result
  for (int i=0; i<OFMAP; i++) {
    result_s[i] = new unsigned char [IMG_SZ]();
  }
  double **arr_o = new double*[OFMAP];
  for (int j=0; j<OFMAP; j++) {
    arr_o[j] = new double[IMG_SZ];
    for (int i=0; i<pad_width*pad_height; i++) {
      arr_o[j][i] = 0.0;
    }
  }
  double **kern = new double*[OFMAP];
  for (int i=0; i<OFMAP; i++) {
    kern[i] = new double[KSIZE*KSIZE];
  }

  double W_3[4][3][3] = {
    {
      { -1, -1, -1 },      // Outline
      { -1,  8, -1 },
      { -1, -1, -1 }
    },
    {
      {  0, -1,  0 },      // Sharpen
      { -1,  5, -1 },
      {  0, -1,  0 }
    },
    {
      {  0.0625,  0.125,  0.0625 },  // Gaussian Blur
      {  0.125,   0.25,   0.125 },
      {  0.0625,   0.125,   0.0625 }
    },
    {
      { -2, -1, 0 },  // Emboss
      { -1,  1, 1 },
      {  0,  1, 2 }
    }
  };
  // 5x5 Kernel for Gaussian blur
  double W_5[1][5][5] = {
    {
      {  1/256.0,  4/256.0,  6/256.0,  4/256.0,  1/256},
      {  4/256.0, 16/256.0, 24/256.0, 16/256.0,  4/256},
      {  6/256.0, 24/256.0, 36/256.0, 24/256.0,  6/256},
      {  4/256.0, 16/256.0, 24/256.0, 16/256.0,  4/256},
      {  1/256.0,  4/256.0,  6/256.0,  4/256.0,  1/256}
    }
  };

  double W_D[3][3] = {{1,1,1}, {0,0,0}, {0,0,-1}};

  // ------------------------ Software Reference Model -------------------------
  double *arr_buff = new double[IMG_SZ];

  // Build kernel weights array for software implementation
  for (int j=0; j<OFMAP; j++) {
    for (int i=0; i<KSIZE*KSIZE; i++) {
      if (KSIZE == 3) {
        kern[j][i] = W_3[FILTER_OPTION][i/KSIZE][i%KSIZE]/IFMAP;
      } else if (KSIZE == 5) {
        kern[j][i] = W_5[FILTER_OPTION][i/KSIZE][i%KSIZE]/IFMAP;
      }
    }
  }

  // Fill feature data input array for software implementation
  unsigned long int counter = 0;
  for (int j=0; j<IFMAP; j++) { // number of channels (1 for monochrome, 3 for RGB)
    for (int i=0; i<pad_height; i++) {
      for(int k=0;k<pad_width;k++) {
        if((i >= ZERO_PAD & i < height+ZERO_PAD) & (k >= ZERO_PAD & k < width+ZERO_PAD)) {
          arr[j][i*pad_width + k] = rarray[j][counter++]/255.0;
        }
        else {
          arr[j][i*pad_width + k] = 0;
        }
      }
    }
  }

  // Software-generated result
  unsigned int O_W = ((pad_width-KSIZE)/stride+1);
  unsigned int O_H = ((pad_height-KSIZE)/stride+1);
  for (int k=0; k<OFMAP; k++) {
    for (unsigned int i=0; i<IFMAP; i++) {
      conv(arr[i], kern[k], arr_buff, pad_height, pad_width, KSIZE, stride);
      for (unsigned int j=0; j<O_W*O_H; j++) {
        arr_o[k][j] += arr_buff[j];
      }
    }
  }
  // ------------------------ Software Reference Model -------------------------

  // ------------------------ DUT ----------------------------------------------
  // Allocate objects for DUT interface
  ac_channel<packedFData<pxlType,IFMAP> > dataIn;
  pxlType weightIn[WC][WR*4];
  ac_channel<packedFData<pxlvldType,OFMAP> > psumOut;

  // Create instance of DUT
  imageProcessor<pxlType, pxlvldType, configType, WIDTH, HEIGHT, KSIZE,IFMAP, OFMAP, ZERO_PAD> imageProcInst;

  // Build kernel weights array for configuring hardware
  for (int j=0; j<4; j++) {
    for (int k=0; k<KSIZE; k++) {
      for (int i=0; i<KSIZE*IFMAP; i++) {
        if (KSIZE == 3) {
          weightIn[i][j*KSIZE + k] = W_3[j][(IFMAP*KSIZE-1-i)%KSIZE][k]/IFMAP;
        } else if (KSIZE == 5) {
          weightIn[i][j*KSIZE + k] = W_5[0][(IFMAP*KSIZE-1-i)%KSIZE][k]/IFMAP;
        }
      }
    }
  }

  // Build configuration data array and write into configuration input channel
  configType config[6] = {stride, width, height, pad_width*KSIZE, (pad_width)*(pad_height), FILTER_OPTION};

  ac_int<62, false> configIn;
  ac_channel<ac_int<62, false> > configWrite;
  configIn.set_slc(0,   (ac_int<3, false>)config[0]);   // 3-bit
  configIn.set_slc(3,   (ac_int<11, false>)config[1]); // 11-bit   
  configIn.set_slc(14,  (ac_int<11, false>)config[2]); // 11-bit
  configIn.set_slc(25,  (ac_int<13, false>)config[3]); // 13-bit
  configIn.set_slc(38,  (ac_int<22, false>)config[4]); // 22-bit
  configIn.set_slc(60,  (ac_int<2, false>)config[5]); // 2-bit
  configWrite.write(configIn);
  // Fill dataIn channel with feature data (packing IFMAP number of feature channels into array)
  for (int i=0; i<width*height; i++) {
    packedFData<pxlType,IFMAP> tmp;
    for (int k=0; k<IFMAP; k++) {
      tmp.data[k] = (pxlType)(rarray[k][i]/255.0);
    }
    dataIn.write(tmp);
  }

  // Run feature data through DUT
  imageProcInst.run(dataIn, configWrite,  psumOut);
  // ------------------------ DUT ----------------------------------------------

  unsigned long int cnt[OFMAP];
  double err[OFMAP];
  for (int i=0; i<OFMAP; i++) {
    cnt[i] = 0;
    err[i] = 0.0;
  }

  packedFData<pxlvldType,OFMAP> temp;
  while (psumOut.available(1)) {
    temp = psumOut.read();
    for (int j=0; j<OFMAP; j++) {
      if (temp.data[j].vld) {
        double tmp1 = temp.data[j].data.to_double(); // convert hardware output type to double
        if (tmp1 < 0) { tmp1 = 0; }
        double tmp2 = arr_o[OFMAP-1-j][cnt[j]]; // get software output reference
        if(tmp2 < 0) { tmp2 = 0; }

        err[j] += fabs(tmp1 -  tmp2);
        result_h[j][cnt[j]] = (unsigned int)ceil((tmp1*255.0)) > 255 ? 255:(unsigned int)ceil((tmp1*255.0));
        result_s[j][cnt[j]] = (unsigned int)ceil((tmp2*255.0)) > 255 ? 255:(unsigned int)ceil((tmp2*255.0));
        cnt[j]++;
      }
    }
  }
  for (int j=0; j<OFMAP; j++) {
    cout << cnt[j] << " " << err[j]/cnt[j] << endl;
    char f_tmp[30];
    sprintf(f_tmp,"%s_%d%s",string(f_name).c_str(),j,string("_hw.bmp").c_str());
    cout << "Writing hardware-generated image file: '" << f_tmp << "'" << endl;
    bmp_24_write(f_tmp,((pad_width-KSIZE)/stride+1), ((pad_height-KSIZE)/stride+1),result_h[j],result_h[j],result_h[j]);

    sprintf(f_tmp,"%s_%d%s",string(f_name).c_str(),j,string("_sw.bmp").c_str());
    cout << "Writing software-generated image file: '" << f_tmp << "'" << endl;
    bmp_24_write(f_tmp,((pad_width-KSIZE)/stride+1), ((pad_height-KSIZE)/stride+1),result_s[j],result_s[j],result_s[j]);
  }

  delete[] arr_buff;
  for (int i=0; i<OFMAP; i++) {
    delete[] result_h[i];
    delete[] result_s[i];
    delete[] kern[i];
    delete[] arr_o[i];
  }
  delete[] result_h;
  delete[] result_s;
  delete[] kern;
  delete[] arr_o;
  for (int i=0; i<IFMAP; i++) {
    delete[] arr[i];
  }
  delete[] arr;
  for (int i=0; i<3; i++) {
    delete[] rarray[i];
  }
  delete[] rarray;

  CCS_RETURN(0);
}

