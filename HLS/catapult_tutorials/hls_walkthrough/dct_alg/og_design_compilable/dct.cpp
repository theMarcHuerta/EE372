//
// Copyright 2003-2015 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
// 
#include "dct.h"

// ---------------------------------------------------
// DCT 2D 8x8 Top
void dct(char *input[8], short *output[8]) {
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

   int mem[8][8];
   int acc;

   ROW0:for (int i=0; i < 8; ++i )
      COL0:for (int j=0; j < 8; ++j ) {
         acc = 0;
         inner1:for (int k=0; k < 8; ++k )
            acc += input[i][k] * coeff[j][k];
         mem[j][i] = acc >> 5;
      }

   ROW1:for (int i=0 ; i < 8; ++i )
      COL1:for (int j=0; j < 8; ++j ) {
         acc = 0;
         inner2:for (int k=0 ; k < 8 ; ++k )
            acc += coeff[i][k] * mem[j][k];
         output[i][j] = acc >> 15 ;
      }
}
