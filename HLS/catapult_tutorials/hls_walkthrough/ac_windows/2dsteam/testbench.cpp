//
// Copyright 2003-2015 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
// 

////////////////////////////////////////////////////////////////////////////////
//  Source:         testbench.cpp
//  Description:    2-d flag windowing example using a sobel edge detector testbench
////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <ac_int.h>
#include "sobel.h"
#include <math.h>

#include <mc_scverify.h>

CCS_MAIN(int argc, char *argv[])
{
  int din[GRID][GRID];
  dType dinImg[GRID*GRID];
  dType doutImg[GRID*GRID];
  int dout_orig[GRID][GRID];

  ac_channel<int> ccso;
  ac_channel<int> refo;  
  int err = 0;
 
  int testnoIn = 0;
  int testnoOut = 0;
  int pelInCnt = 0;
  int pelOutCnt = 0;
  bool rampup = false;
	

for(int l=0;l<3;l++){
  for(int i=0;i<GRID;i++)
    for(int j=0;j<GRID;j++){
      din[i][j] = i*GRID + j + l;
    }
  printf("Start processing \n");    

  sobel_orig(din,dout_orig,GRID,GRID);
  for(int i=0;i<GRID;i++)
    for(int j=0;j<GRID;j++){
      refo.write(dout_orig[i][j]);
    }
 
  CCS_DESIGN(sobel)(din,ccso,GRID,GRID);
 
  while(ccso.size() && refo.size()) {
    int ccst = ccso.read();
    int reft = refo.read();
    if (ccst!=reft) {
      err++;
    }
#ifdef DEBUG
    printf("%2x  %2x  ", ccst, reft);
    if (ccst!=reft) {
      printf("#\n");
    } else {
      printf("=\n");
    }
#endif
  }
	

  printf("test passed with %d error(s)...\n", err);
  if (!err) printf("OK!\n");
  else CCS_RETURN(-1);
 }
 CCS_RETURN(0);
}
