//
// Copyright 2003-2015 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
// 

#include "transpose_filter.h"
#include "mc_scverify.h"

CCS_MAIN(int argv, char *argc){
  ac_channel<uint8 > din; 
  ac_channel<uint8 > dout;
  int height,width;
  static  uint8 mem[64][64];

  height = 4;
  width = 4;

  for(int i=0;i<4;i++){
    printf("\nInput data\n\n");

    din.write(width);
    din.write(height);
    ROW:for(int r=0; r<64; r++){
      COL:for(int c=0; c<64; c++){
        mem[r][c] = r*width + c + 1;
        din.write(mem[r][c]); 
        printf("%2d ", mem[r][c].to_uint()); 
        if(c == width -1)
          break;
      }
      printf("\n");
      if(r == height -1)
        break;
    }

    CCS_DESIGN(transpose_and_filter)(din,dout);

    printf("\nOutput data\n\n");
 
    ROW1:for(int r=0; r<64; r++){
      COL1:for(int c=0; c<64; c++){
      
        printf("%2d ", dout.read().to_uint()); 
        if(c == width -1)
          break;
      }
      printf("\n");
      if(r == height -1)
        break;
    }

  }
  printf("\n");
  CCS_RETURN(0);
}
