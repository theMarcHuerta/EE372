//
// Copyright 2003-2015 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
// 

////////////////////////////////////////////////////////////////////////////////
//  Source:         sobel_orig.cpp
//  Description:    Sobel reference 
////////////////////////////////////////////////////////////////////////////////
#include <ac_int.h>
#include "sobel.h"
int clipo(int i, int sz){
  int tmp;    
  if(i<0)
    tmp = 0;
  else if(i > sz-1)
    tmp = sz-1;
  else
    tmp = i;
  return tmp;     
}


int mirror(int i, int sz){
    
 return ((i<0)?(i*-1):((i<sz)?i:2*sz-i-2));
}

int bounds(int i, int sz){
#ifdef MIRROR
	return mirror(i,sz);
#endif

#ifdef CLIP
	return clipo(i,sz);
#endif
}
int abso(int i){
  if(i<0)
    return -i;
  else
    return i;
}
const int cx[3][3] = {-1,0,1,-2,0,2,-1,0,1};
const int cy[3][3] = {1,2,1,0,0,0,-1,-2,-1};

void sobel_orig(dType din[GRID][GRID], dType dout[GRID][GRID], int row_sz, int col_sz){
  int Gx,Gy;
  int tmpx,tmpy;

 ROW:for(int i=0;i<row_sz;i++){
  COL:for(int j=0;j<col_sz;j++){
      tmpx = 0;
      tmpy = 0;
      for(int r=0;r<3;r++){
        for(int c=0;c<3;c++){
          tmpx += din[bounds(i+r-1,GRID)][bounds(j+c-1,GRID)]*cx[r][c];
          tmpy += din[bounds(i+r-1,GRID)][bounds(j+c-1,GRID)]*cy[r][c];
        }
      }
      Gx = tmpx;
      Gy = tmpy;
      dout[i][j] = abso(Gx) + abso(Gy); 
    }

  }
}
