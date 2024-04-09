//
// Copyright 2003-2015 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
// 

////////////////////////////////////////////////////////////////////////////////
//  Source:         ac_window_1d_array.h
//  Description:    1-d array windowing example
////////////////////////////////////////////////////////////////////////////////
//#define DEBUG
//#define MIRROR
#ifndef __test_h__
#define __test_h__

#include "ac_int.h"
#include "ac_window.h"


typedef unsigned char	uchar;

#define MAXSZ 100
#define clip(i, sz) ((i<0)?0:((i<sz)?i:sz-1))

typedef struct{
  uchar data;
  bool sof;
  bool eof;  
  bool sol;
  bool eol;
}  dataWithFlags;


void window_ref (int linei[MAXSZ], int lineo[MAXSZ] );

void window_ccs(int din[MAXSZ], int dout[MAXSZ]);
#endif
