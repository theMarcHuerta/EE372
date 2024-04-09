//
// Copyright 2003-2015 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
// 

#include <ac_int.h>
#include <ac_channel.h>

#pragma hls_design top
void avg2 (ac_channel<uint4 > &din, 
           ac_channel<uint4 > &dout) {

  uint5 sum = 0;//Initialize variables to avoid Unitialized Memory Reads (UMR)

  ACC:for (int i=0; i<2; i++) {
    #ifndef __SYNTHESIS__
    printf("Reading data input channel\n");
    #endif

    sum += din.read();
  }

  #ifndef __SYNTHESIS__
  printf("Writing data output channel\n");
  #endif

  dout.write(sum);
}
