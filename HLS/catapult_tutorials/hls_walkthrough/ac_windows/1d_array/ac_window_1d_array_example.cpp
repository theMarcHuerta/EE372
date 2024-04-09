//
// Copyright 2003-2015 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
// 

////////////////////////////////////////////////////////////////////////////////
//  Source:         1d_array_window.cpp
//  Description:    1-d array windowing example
////////////////////////////////////////////////////////////////////////////////
#include "ac_window_1d_array_example.h"
int mirror(int i, int sz){
    
  return ((i<0)?(i*-1):((i<sz)?i:2*sz-i-2));
}
void window_ref (int linei[MAXSZ], int lineo[MAXSZ] )
{
  int ltmp[MAXSZ];  
  for (int i=0; i<MAXSZ; i++) {
    ltmp[i] = (        linei[mirror(i-2, MAXSZ)] +
                       linei[mirror(i-1, MAXSZ)] +
                       linei[mirror(i  , MAXSZ)] +
                       linei[mirror(i+1, MAXSZ)] +
                       linei[mirror(i+2, MAXSZ)]
                       );
  }
  for (int i=0; i<MAXSZ; i++) {
    lineo[i] = (        ltmp[mirror(i-2, MAXSZ)] +
                        ltmp[mirror(i-1, MAXSZ)] +
                        ltmp[mirror(i  , MAXSZ)] +
                        ltmp[mirror(i+1, MAXSZ)] +
                        ltmp[mirror(i+2, MAXSZ)]
                        );
#ifdef DEBUG
    printf("REF / %2x %2x %2x %2x %2x\n", ltmp[mirror(i-2, MAXSZ)],
           ltmp[mirror(i-1, MAXSZ)],
           ltmp[mirror(i  , MAXSZ)],
           ltmp[mirror(i+1, MAXSZ)],
           ltmp[mirror(i+2, MAXSZ)] );
#endif

  }
}

#pragma design top
void window_ccs(int din[MAXSZ], int dout[MAXSZ])
{
  int ltmp[MAXSZ];   
  ac_window_1d_array<int, 5, MAXSZ, AC_MIRROR> w0(din,MAXSZ);

  for(int i=w0.pos();i<MAXSZ; i++){      
    w0++;
    if(w0.valid()){
      ltmp[i] = (w0[-2] + w0[-1] + w0[+0] + w0[+1] + w0[+2] );
        
    }
  }
  ac_window_1d_array<int, 5, MAXSZ, AC_MIRROR> w1(ltmp,MAXSZ);
  for(int i=w1.pos();i<MAXSZ;i++){
    w1++;
    if(w1.valid()){
      dout[i] = (w1[-2] + w1[-1] + w1[+0] + w1[+1] + w1[+2] );
#ifdef DEBUG

      printf("CCS / %2x %2x %2x %2x %2x\n", w1[-2], w1[-1], w1[+0], w1[+1], w1[+2]);
#endif  
    }
  }             
}

