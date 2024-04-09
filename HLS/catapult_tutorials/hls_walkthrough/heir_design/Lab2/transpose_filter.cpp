//
// Copyright 2003-2015 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
// 

#include <ac_channel.h>
#include <ac_int.h>
#include "shift_class.h"
#include "transpose_filter.h"

#pragma hls_design //Design hierarchy pragma
void transpose(ac_channel<uint8 > &din, ac_channel<memStruct > &dout, 
	       ac_channel<uint7 > &height_out, ac_channel<uint7 > &width_out){
  ac_int<7,false> height,width;
  memStruct mem;//temporary storage for shared memory
        
  //Sequential channel reads from "din" will limit pipelining at the top-level
  //First two values read from "din" are configuration data for height and width
  width = din.read();//Make an internal copy of configuration data to limit channel reads 
  height = din.read();
  width_out.write(width);//Copy configuration data to next block      
  height_out.write(height);

  ROW:for(int r=0; r<64; r++){//ROW can be pipelined with II=1
    COL:for(int c=0; c<64; c++){
      mem.data[c][r] = din.read();//Single channel read
      if(c == width -1)
        break;
    }
    if(r == height -1)
      break;
  }
  dout.write(mem);
}

#pragma map_to_operator [CCORE]//Force muxing to be a single object by making it a CCORE
void clip_window( shift_class<uint8, 3> shift_reg, 
                  uint6 i, uint7 width, uint8 window[3]){

  window[0] = (i==1) ? shift_reg[1]:shift_reg[2];
  window[1] = shift_reg[1];
  window[2] = (i==width) ? shift_reg[1]:shift_reg[0];
}

#pragma hls_design 
void filter(ac_channel<memStruct > &din, ac_channel<uint8 > &dout, 
	    ac_channel<uint7 > &height_in, ac_channel<uint7 > &width_in){
  uint7 height,width;
  memStruct mem;//Temporary storage for shared memory

  shift_class<uint8, 3> shift_reg;
  uint8 window[3];
  uint8 mac;
  uint8 din_tmp;

  height = height_in.read();//Make local copy of control signals so there is only one channel read
  width = width_in.read();

  mem = din.read();//Read memory data into temporary struct

  ROW:for(int r=0; r<64; r++){
    COL:for(int c=0; c<64 + 1; c++){
      if(c<width)//prevent over-read of din 
        din_tmp = mem.data[r][c];
      shift_reg << din_tmp;//Sliding window storage of din
      clip_window(shift_reg,c,width,window);//Boundary processing
      //      printf("0:%d 1:%d 2:%d\n", window[0].to_uint(), window[1].to_uint(), window[3].to_uint());
      mac = window[0]/4 + window[1]/2 + window[2]/4;
      if(c>=1) {//startup
        dout.write(mac);
	//	printf("mac:%d\n",mac.to_int());
      }
      if(c == width)
        break;
    }
    if(r == height-1)
      break;
  }
}

#pragma hls_design top
void transpose_and_filter(ac_channel<uint8 > &din, ac_channel<uint8 > &dout){
  //Static channels at top-level used to interconnect hierarchical blocks
  static ac_channel<memStruct > mem;//Shared memory object
  static ac_channel<uint7 > height;//Control channels
  static ac_channel<uint7 > width;

  transpose(din,mem,height,width);
  filter(mem,dout,height,width);
}
