//
// Copyright 2003-2015 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
// 

#ifndef DCT_H_
#define DCT_H_
#include <ac_channel.h>
#include <ac_int.h>
#include "jpeg.h"


struct memStruct{//Container for array passed through channel
  ac_int<16> data[8][8];
};

void dct(ac_channel<ac_int<8> > &input, ac_channel<memStruct > &output);
#endif /* DCT_H_ */
