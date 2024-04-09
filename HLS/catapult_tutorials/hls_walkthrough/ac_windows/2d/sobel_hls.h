//*****************************************************************************************
// File: sobel_hls.h
//
// Description: Implements a Sobel edge detect using ac_window_2d_flag.
//
// Template Parameters:
//    pType - configures the data type of the input and output data
//    accuType - configures the data type of the internal accumulator variables
//    IMG_W, IMG_H - configures the maximum width/height of the input data
//    K_R, K_C - configures the kernel size (rows and columns)
//    AC_MODE - configures the windowing mode (AC_MIRROR or AC_CLIP)
//
// Public Interface:
//    The interface to the Sobel is the 'run' member function.
//
// Revision History:
//     2019-06-24 - dgb - Initial version built from older examples
//
//*****************************************************************************************

#ifndef _INCLUDED_SOBEL_HLS_H_
#define _INCLUDED_SOBEL_HLS_H_

#include <ac_int.h>
#include <ac_window.h>
#include "sobel.h"

// Include header for SCVerify support (CCS_BLOCK macro)
#include <mc_scverify.h>

template <class pType, class accuType, class kType, int IMG_W, int IMG_H, int K_R, int K_C, int AC_MODE>
class sobel
{
public:
  sobel() {}

  #pragma hls_design interface
  void CCS_BLOCK(run)(dataWithFlags<pType> &din, dataWithFlags<pType> &dout, kType cx[K_R][K_C], kType cy[K_R][K_C])
  {
    bool sofOut;  // Start of frame output control
    bool eofOut;  // End of frame output control
    bool solOut;  // Start of line output control
    bool eolOut;  // End of line output control
    accuType Gx = 0;
    accuType Gy = 0;
    accuType tmpx,tmpy;
    tmpx = tmpy = 0;
    w.write(din.data,din.sof,din.eof,din.sol,din.eol);    // write all data and control inputs
#ifdef DEBUG
    if (w.valid()) {
      // Print the window contents
      printf("sobel_hls()\n");
      for (int r=0; r<K_R; r++) {
        for (int c=0; c<K_C; c++) {
          printf("%5d ",w(r-K_R/2,c-K_C/2));
        }
        printf("\n");
      }
      printf("\n");
    }
#endif
    if (w.valid()) {
      for (int r=0; r<K_R; r++) {
        for (int c=0; c<K_C; c++) {
          tmpx += w(r-K_R/2,c-K_C/2)*cx[r][c];
          tmpy += w(r-K_R/2,c-K_C/2)*cy[r][c];
        }
      }
    }

    Gx = (tmpx<0) ? (accuType)-tmpx : tmpx;
    Gy = (tmpy<0) ? (accuType)-tmpy : tmpy;
    w.readFlags(sofOut,eofOut,solOut,eolOut);     // read output control flags
    dout.data = Gx + Gy;
    dout.sof = sofOut;
    dout.eof = eofOut;
    dout.sol = solOut;
    dout.eol = eolOut;

#ifdef DEBUG
    printf("flags sof = %d eof = %d sol = %d eol = %d\n",sofOut,eofOut,solOut,eolOut);
#endif
  }

private:
  ac_window_2d_flag<int,K_R,K_C,IMG_W,AC_MODE>     w;  // Instance of the windowing class
};

#endif

