//
// Copyright 2003-2015 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
// 
//Folded filter because of symmetrical coefficients

#include "dct.h"
#include <mc_scverify.h>

template<typename T0, typename T1,typename T2>
T2 mult_add(T0 data[8], ac_int<3,false> i) {
   T2 acc = 0;
   T1 pa[4];
   const ac_int<10> coeff[8][4] = {
         {362,  362,  362,  362},
         {502,  425,  284,   99},
         {473,  195, -195, -473},
         {425,  -99, -502, -284},
         {362, -362, -362,  362},
         {284, -502,   99,  425},
         {195, -473,  473, -195},
         { 99, -284,  425, -502}
   };

   PRE_ADD:for (int k=0 ; k < 4 ; ++k ) {
      pa[k] = data[k] + ((i&1)? (int)-data[7-k]:(int)data[7-k]);
   }

   MAC:for (int k=0 ; k < 4 ; ++k ) {
      acc += coeff[i][k] * pa[k];
   }
   return acc;
}

#pragma design
void CCS_BLOCK(dct_h)(ac_channel<ac_int<8> > &input, ac_channel<memStruct > &mem) {//Memory interfaces must use ac_channel coding style
   memStruct local_mem;//Local struct for temporary array operations
   ac_int<8> buf0[8];//Local storage for one row of input

   ac_int<21> acc0;
   ROW0:for (int i=0; i < 8; ++i ) {
      COPY_ROW0:for (int p=0; p<8; p++)//Copy one row of input into local storage
         buf0[p] = input.read();
      COL0:for (int j=0; j < 8; ++j ) {
         acc0 = mult_add<ac_int<8>,ac_int<9>,ac_int<21> >(buf0,j);
         local_mem.data[j][i] = acc0 >> 5;
      }
   }
   mem.write(local_mem);
}

#pragma design
void CCS_BLOCK(dct_v)(ac_channel<memStruct > &mem, ac_channel<memStruct > &output) {//Memory interfaces must use ac_channel coding style
   memStruct local_mem;//Local struct for temporary array operations
   memStruct local_output;


   ac_int<16> buf1[8];//Local storage for one row of input
   ac_int<31> acc1;

   local_mem = mem.read();
   COL1:for (int j=0; j < 8; ++j ) {
      COPY_ROW:for (int p=0; p<8; p++)//Copy one row of mem into local storage
         buf1[p] = local_mem.data[j][p];
      ROW1:for (int i=0 ; i < 8; ++i ) {
         acc1 = mult_add<ac_int<16>,ac_int<17>,ac_int<31> >(buf1,i);
         local_output.data[i][j] = acc1 >> 15 ;
      }
   }
   output.write(local_output);


}
// ---------------------------------------------------
// DCT 2D 8x8 Top
void CCS_BLOCK(dct)(ac_channel<ac_int<8> > &input, ac_channel<memStruct > &output) {//Memory interfaces must use ac_channel coding style
   static ac_channel<memStruct > mem;//Static interconnect channel

    //Top-level design only instantiates hierarchical blocks
   dct_h(input,mem);
   dct_v(mem,output);
}
