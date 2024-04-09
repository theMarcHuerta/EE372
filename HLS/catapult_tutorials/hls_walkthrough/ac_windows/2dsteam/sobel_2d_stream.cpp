//
// Copyright 2003-2015 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
// 

////////////////////////////////////////////////////////////////////////////////
//  Source:         sobel_2d_stream.cpp
//  Description:    2-d stream windowing example using a sobel edge detector
////////////////////////////////////////////////////////////////////////////////
#include <ac_int.h>
#include "sobel.h"

const int cx[3][3] = {-1,0,1,-2,0,2,-1,0,1};
const int cy[3][3] = {1,2,1,0,0,0,-1,-2,-1};

int abs(int i){
  if(i<0)
    return -i;
  else
    return i;
}
#include "ac_window_2d_stream.h"
#pragma design top
void sobel(int din[GRID][GRID], ac_channel<int> &dout, int row_sz, int col_sz){
#ifdef CLIP
  static ac_window_2d_stream<int,WN_SZ,WN_SZ,GRID,GRID,AC_CLIP> w;
#endif
#ifdef MIRROR
  static ac_window_2d_stream<int,WN_SZ,WN_SZ,GRID,GRID,AC_MIRROR> w;
#endif

  int Gx,Gy;
  int tmpx,tmpy;

  w.write(&din[0][0],row_sz,col_sz);    //write all data and control inputs

  for(int i=0;i<GRID;i++){
    for(int j=0; j<GRID;j++){
      w++;
      tmpx = tmpy = 0;
      if(w.valid()){
        for(int r=0;r<WN_SZ;r++){
          for(int c=0;c<WN_SZ;c++){
            tmpx += w(r-WN_SZ/2,c-WN_SZ/2)*cx[r][c];
            tmpy += w(r-WN_SZ/2,c-WN_SZ/2)*cy[r][c];
          } 
        }
        Gx = tmpx;
        Gy = tmpy;
        dout.write(abs(Gx) + abs(Gy));
      }
      if(j==col_sz-1)
        break;
    }
    if(i==row_sz-1)
      break;
  }
}
