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
//  Description:    2-d flag windowing example using a sobel edge detector
////////////////////////////////////////////////////////////////////////////////
#ifndef _SOBEL
#define _SOBEL
#define MIRROR
#include <ac_int.h>
#include <ac_channel.h>
#include "ac_window.h"
#define DEBUG
#define GRID 32
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

void sobel(int din[GRID][GRID], ac_channel<int> &dout);
void sobel_orig(dType din[GRID][GRID], dType dout[GRID][GRID]);
#endif
