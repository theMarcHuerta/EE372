//
// Copyright 2003-2015 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
//
//Quantized algoritm using bit-accurate data types.
#include "dct.h"
#include <mc_scverify.h>

// ---------------------------------------------------
// DCT 2D 8x8 Top
void CCS_BLOCK(dct)(ac_channel<ac_int<8> > &input, short output[8][8]) {
   const int coeff[8][8] = {
         {362,  362,  362,  362,  362,  362,  362,  362},
         {502,  425,  284,   99,  -99, -284, -425, -502},
         {473,  195, -195, -473, -473, -195,  195,  473},
         {425,  -99, -502, -284,  284,  502,   99, -425},
         {362, -362, -362,  362,  362, -362, -362,  362},
         {284, -502,   99,  425, -425,  -99,  502, -284},
         {195, -473,  473, -195, -195,  473, -473,  195},
         { 99, -284,  425, -502,  502, -425,  284,  -99}
   };

   ac_int<16> mem[8][8];
   ac_int<8> buf[8];//Local storage for one row of input
   ac_int<21> acc0;
   ac_int<31> acc1;

   ROW0:for (int i=0; i < 8; ++i ) {
      COPY_ROW:for (int p=0; p<8; p++)//Copy one row of input into local storage
         buf[p] = input.read();
      COL0:for (int j=0; j < 8; ++j ) {
         acc0 = 0;
         inner1:for (int k=0; k < 8; ++k )
            acc0 += buf[k] * coeff[j][k];
         mem[j][i] = acc0 >> 5;
      }
   }

   ROW1:for (int i=0 ; i < 8; ++i )
      COL1:for (int j=0; j < 8; ++j ) {
         acc1 = 0;
         inner2:for (int k=0 ; k < 8 ; ++k )
            acc1 += coeff[i][k] * mem[j][k];
         output[i][j] = acc1 >> 15 ;
      }
}