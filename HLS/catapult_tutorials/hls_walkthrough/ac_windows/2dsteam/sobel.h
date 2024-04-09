//
// Copyright 2003-2015 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
// 

////////////////////////////////////////////////////////////////////////////////
//  Source:         sobel.h
//  Description:    sobel edge detector header file
////////////////////////////////////////////////////////////////////////////////
#ifndef _SOBEL
#define _SOBEL
#include <ac_int.h>
#include <ac_channel.h>
#define MIRROR
#define GRID 100
#define WN_SZ 3
#define INTTYPE
#ifdef INTTYPE
typedef int dType;
#else
typedef ac_int<8,false> dType;
#endif
typedef struct{
  dType data;
  ac_int<1,false> sof;
  ac_int<1,false> eof;  
  ac_int<1,false> sol;
  ac_int<1,false> eol;
}  dataWithFlags;

void sobel_orig(dType din[GRID][GRID], dType dout[GRID][GRID], int row_sz, int col_sz);
void sobel(int din[GRID][GRID], ac_channel<int> &dout, int row_sz, int col_sz);
#endif
