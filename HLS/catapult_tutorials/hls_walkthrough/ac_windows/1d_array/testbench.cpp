//
// Copyright 2003-2015 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
// 

////////////////////////////////////////////////////////////////////////////////
//  Source:         ac_window_1d_array.cpp
//  Description:    1-d array windowing example testbench
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include "ac_window_1d_array_example.h"

#include <mc_scverify.h>

CCS_MAIN(int argc, char *argv[])
{
  int linei[MAXSZ];
  int lineo[MAXSZ];
  int ccso[MAXSZ];

  int err = 0;
  int lsz[3] = {8, 16, 18};
  for (int testno=0; testno<3; testno++) {

    for (int i=0; i<MAXSZ; i++) {
      linei[i]=i+1+testno*128;
    }
    CCS_DESIGN(window_ccs)(linei,  ccso);

    window_ref(linei, lineo);

                
    for (int i=0; i<MAXSZ; i++) {
      if (ccso[i]!=lineo[i]) {
        err++;
      }
#ifdef DEBUG
      printf("%2x  %2x  ", ccso[i], lineo[i]);
      if (ccso[i]!=lineo[i]) {
        printf("#\n");
      } else {
        printf("=\n");
      }
#endif
    }
  }

  printf("test passed with %d error(s)...\n", err);
  if (!err) printf("OK!\n");
  else CCS_RETURN(-1);
  CCS_RETURN(0);
}
