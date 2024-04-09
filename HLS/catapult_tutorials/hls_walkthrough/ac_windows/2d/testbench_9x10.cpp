//*****************************************************************************************
// File: testbench_3x3.cpp
//
// Description: Testbench for exercising the Sobel edge detect design in sobel_hls.h
//     comparing against the algorthmic version in sobel_orig.h.
//     This example illustrates using an even size kernel (9x10)
//     Input data is 32 bit signed integer.
//     Kernel data is 3 bit signed integer.
//     Reference model is all 'int'.
//
// Revision History:
//     2019-06-24 - dgb - Initial version built from older examples
//
//*****************************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <ac_int.h>

#include "sobel.h"
#include "sobel_orig.h"
#include "sobel_hls.h"

#include <math.h>

#include <mc_scverify.h>

CCS_MAIN(int argc, char *argv[])
{
  //--------------------------------------------------------------------------------------
  // Configuration values for testbench
  enum {
    num_iter     = 3,         // This enum configures how many frames will be tested
    IMG_W        = 200,       // These enums configure the maximum image size
    IMG_H        = 200,
    K_R          = 9,         // These enums configure the Kernel size (even width kernel)
    K_C          = 10,
    BMODE_MIRROR = 0,         // These enums define symbolic values for the algorithmic version's boundary mode
    BMODE_CLIP   = 1
  };

  //--------------------------------------------------------------------------------------
  // Datatypes for reference design
  typedef int               ref_kType;
  typedef int               ref_dType;
  typedef int               ref_accuType;

  sobel_orig<ref_dType,ref_accuType,IMG_W,IMG_H,K_R,K_C,BMODE_MIRROR>    orig_INST;  // reference model

  // Sobel kernel data
  const ref_kType ref_cx[K_R][K_C]    = { -1,  0,  1, -2,  0, -1,  0,  1, -2,  0, 
                                           1,  2, -1,  0,  1,  1,  2, -1,  0,  1,
                                          -1,  0,  1, -2,  0, -1,  0,  1, -2,  0,
                                           1,  2, -1,  0,  1,  1,  2, -1,  0,  1,
                                          -1,  0,  1, -2,  0, -1,  0,  1, -2,  0,
                                           1,  2, -1,  0,  1,  1,  2, -1,  0,  1,
                                          -1,  0,  1, -2,  0, -1,  0,  1, -2,  0,
                                           1,  2, -1,  0,  1,  1,  2, -1,  0,  1,
                                          -1,  0,  1, -2,  0, -1,  0,  1, -2,  0
                                        };
  const ref_kType ref_cy[K_R][K_C]    = {  1,  2,  1,  0,  0,  1,  2,  1,  0,  0,
                                          -1, -1, -2, -1, -1, -1, -1, -2, -1, -1,
                                           1,  2,  1,  0,  0,  1,  2,  1,  0,  0,
                                          -1, -1, -2, -1, -1, -1, -1, -2, -1, -1,
                                           1,  2,  1,  0,  0,  1,  2,  1,  0,  0,
                                          -1, -1, -2, -1, -1, -1, -1, -2, -1, -1,
                                           1,  2,  1,  0,  0,  1,  2,  1,  0,  0,
                                          -1, -1, -2, -1, -1, -1, -1, -2, -1, -1,
                                           1,  2,  1,  0,  0,  1,  2,  1,  0,  0
                                       };

  //--------------------------------------------------------------------------------------
  // Datatypes for synthesizable design
  typedef ac_int<3,true>    kType;     // Data type for kernel data
  typedef ac_int<32,true>   dType;      // Data type used for this version of the design
  typedef dType::rt_T<kType>::mult T;  // Derive the data type that can hold result of multiplying 'dType' with an 'int'
  typedef T::rt_unary::set<K_R*K_C>::sum accu; // Derive a data type that can hold the result of accumlating K_RxK_C mult results

  sobel<dType,accu,kType,IMG_W,IMG_H,K_R,K_C,AC_MIRROR>         dut_INST;   // DUT has bit-accurate types

  // Sobel kernel data (constructed from reference kernel data via type conversion ref_kType->kType)
  kType cx[K_R][K_C];
  kType cy[K_R][K_C];
  for (int r=0; r<K_R; r++) for (int c=0; c<K_C; c++) { cx[r][c] = ref_cx[r][c]; cy[r][c] = ref_cy[r][c]; }

  ref_dType din_orig[num_iter][IMG_W][IMG_H];
  ref_dType din_orig_frame[IMG_W][IMG_H];
  ref_dType dout_orig[num_iter][IMG_W][IMG_H];
  ref_dType dout_orig_frame[IMG_W][IMG_H];
  dType dinImg[num_iter*IMG_W*IMG_H];
  dType doutImg[num_iter*IMG_W*IMG_H];

  bool  sol;
  bool  eol;
  bool  sof;
  bool  eof;

  dataWithFlags<dType> din;
  dataWithFlags<dType> dout;

  int sz[5] = {10,10,10,10,10}; // use 10x10 size image for up to 5 frames
  // create num_iter frames of data and pack them for the ac_window_2d_flag implementation
  int icnt=0;
  for (int k=0; k<num_iter; k++) {
    for (int i=0; i<sz[k]; i++) {
      for (int j=0; j<sz[k]; j++) {
        din_orig[k][i][j] = i*sz[k] + j;
        dinImg[icnt++] = din_orig[k][i][j];
      }
    }
  }

  printf("Start processing \n");

  // run three frames through the original design
  for (int k=0; k<num_iter; k++) {
    for (int i=0; i<sz[k]; i++) {
      for (int j=0; j<sz[k]; j++) { // get frame
        din_orig_frame[i][j] = din_orig[k][i][j];
      }
    }
    orig_INST.run(din_orig_frame,dout_orig_frame,sz[k],ref_cx,ref_cy);

    for (int i=0; i<sz[k]; i++) {
      for (int j=0; j<sz[k]; j++) { // store frame
        dout_orig[k][i][j] = dout_orig_frame[i][j];
      }
    }
  }

  int frameNoIn = 0;
  int frameNoOut = 0;
  int pelInCnt = 0;
  int pelOutCnt = 0;
  bool rampup = false;
  int inCnt=0;
  // Run synthesizable window implenetation until all data is processed
  // The window flags are used to control the flow of input and output data
  // NOTE the mirroring and cliping and start of new line are controlled purely by the sol and eol flags
  do {
    if (pelInCnt == 0) {
      sof = 1;
    } else {
      sof = 0;
    }
    if (pelInCnt == sz[frameNoIn]*sz[frameNoIn]-1) {
      eof = 1;
    } else {
      eof = 0;
    }
    if (pelInCnt%sz[frameNoIn]==0) {                   // A start of line sol is generated for each new line
      sol= 1;                          // Set to one on the first pel of the line
    } else {
      sol = 0;
    }
    if (pelInCnt%sz[frameNoIn] == sz[frameNoIn]-1) { // End of line is generated on the last pel of each line
      eol = 1;
    } else {
      eol = 0;
    }

    if (frameNoIn < num_iter) {
      // Data is loaded into a user defined struct
      din.data = dinImg[inCnt++];
    }

    din.sof = sof;
    din.eof = eof;
    din.sol = sol;
    din.eol = eol;

    dut_INST.run(din,dout,cx,cy);

    // wait for prefill of first line
    if (dout.sol) {
      rampup = true;
    }
    // write valid output
    if (rampup) {
      doutImg[pelOutCnt++] = dout.data;
    }
    // start new input line count
    pelInCnt++;

    // Advance to next line size on eof of output
    if (din.eof == 1) {
      frameNoIn++;
      pelInCnt = 0;
    }
    if (dout.eof == 1) {
      frameNoOut++;
    }
  } while (frameNoOut != num_iter);

  icnt = 0;
  int errCnt=0;
  for (int k=0; k<num_iter; k++) {
    for (int i=0; i<sz[k]; i++) {
      for (int j=0; j<sz[k]; j++) {
        int tmp = doutImg[icnt++];
        if (tmp != dout_orig[k][i][j]) {
          errCnt++;
          printf("Mismatch frame = %d row=%d col=%d actual=%d  reference=%d\n",k,i,j, doutImg[icnt-1],dout_orig[k][i][j]);
        }
      }
    }
  }
  printf("Design finished with %d errors\n",errCnt);
  if (errCnt != 0) {
    printf("Changing the line size dynamically will cause mismatches with \n");
    printf("the reference design.  This is caused by the 2-d window ramping up. \n");
    printf("This is a known limitation with the current ac_window_2d_flag class \n");
  }
  CCS_RETURN(errCnt);
}

