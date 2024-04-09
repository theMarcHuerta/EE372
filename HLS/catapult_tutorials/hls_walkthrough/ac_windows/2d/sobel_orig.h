//*****************************************************************************************
// File: sobel_orig.h
//
// Description: Algorithmic description of a Sobel edge detect
//
// Template Parameters:
//    pType - configures the data type of the input and output data
//    accuType - configures the data type of the internal accumulator variables
//    IMG_W, IMG_H - configures the maximum width/height of the input data
//    K_R, K_C - configures the kernel size (rows and columns)
//    BMODE - configures the windowing mode (0 = mirror, 1 = clip)
//
// Public Interface:
//    The interface to the Sobel is the 'run' member function.
//
// Revision History:
//     2019-06-24 - dgb - Initial version built from older examples
//
//*****************************************************************************************

#ifndef _INCLUDED_SOBEL_ORIG_H_
#define _INCLUDED_SOBEL_ORIG_H_

#include "sobel.h"

template <class pType, class accuType, int IMG_W, int IMG_H, int K_R, int K_C, bool BMODE>
class sobel_orig 
{
public:
  sobel_orig() {}

  void run(pType din[IMG_W][IMG_H], pType dout[IMG_W][IMG_H],int sz, const pType cx[K_R][K_C], const pType cy[K_R][K_C])
  {
    accuType Gx,Gy;
    accuType tmpx,tmpy;

#ifdef DEBUG
    // Print the window contents
    printf("sobel_orig()\n");
    R:for (int i=0; i<sz; i++) {
      C:for (int j=0; j<sz; j++) {
        for (int r=0; r<K_R; r++) {
          for (int c=0; c<K_C; c++) {
            printf("%5d ",din[bounds(i+r-K_R/2,sz)][bounds(j+c-K_C/2,sz)]);
          }
          printf("\n");
        }
        printf("\n");
      }
    }
#endif

    ROW:for (int i=0; i<sz; i++) {
      COL:for (int j=0; j<sz; j++) {
        tmpx = 0;
        tmpy = 0;
        for (int r=0; r<K_R; r++) {
          for (int c=0; c<K_C; c++) {
            tmpx += din[bounds(i+r-K_R/2,sz)][bounds(j+c-K_C/2,sz)]*cx[r][c];
            tmpy += din[bounds(i+r-K_R/2,sz)][bounds(j+c-K_C/2,sz)]*cy[r][c];
          }
        }
        Gx = (tmpx<0) ? (accuType)-tmpx : tmpx; // this implements absolute-value
        Gy = (tmpy<0) ? (accuType)-tmpy : tmpy; // this implements absolute-value
        dout[i][j] = Gx + Gy;
      }
    }
  }

private: // helper functions
  int clipo(int i, int sz) {
    int tmp;
    if (i < 0) {
      tmp = 0;
    } else if (i > sz-1) {
      tmp = sz-1;
    } else {
      tmp = i;
    }
    return tmp;
  }

  int mirroro(int i, int sz) {
    return ((i<0)?(i*-1):((i<sz)?i:2*sz-i-2));
  }

  int bounds(int i, int sz) {
    if (BMODE==0) {          // MIRROR
      return mirroro(i,sz);
    } else {                 // CLIP
      return clipo(i,sz);
    }
  }
};

#endif

