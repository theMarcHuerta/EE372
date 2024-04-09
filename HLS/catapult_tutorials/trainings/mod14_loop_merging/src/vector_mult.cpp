//
// Copyright 2018 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
//

#include "vector_mult.h"
#include <mc_scverify.h>
using namespace std;

#pragma hls_design top
void CCS_BLOCK(vector_mult)(
  ac_channel<OP1_TYPE>    &op1_chan,
  ac_channel<OP2_TYPE>    &op2_chan,
  ac_channel<RESULT_TYPE> &result_chan)
{

  OP1_TYPE  op1_mem[VECTOR_SIZE];
  OP2_TYPE  op2_mem[VECTOR_SIZE];
  RESULT_TYPE  result_mem[VECTOR_SIZE];

#ifndef __SYNTHESIS__
  while (op1_chan.available(VECTOR_SIZE) && op2_chan.available(VECTOR_SIZE))
#endif
  {
    LOAD_OP1:for (unsigned i=0; i<VECTOR_SIZE; i++) {
      op1_mem[i] = op1_chan.read();
    }

    LOAD_OP2:for (unsigned i=0; i<VECTOR_SIZE; i++) {
      op2_mem[i] = op2_chan.read();
    }

    VECTOR_MULT:for (unsigned i=0; i<VECTOR_SIZE; i++) {
      result_mem[i] = op1_mem[i] * op2_mem[i];
    }

    UNLOAD_RESULT:for (unsigned i=0; i<VECTOR_SIZE; i++) {
      result_chan.write(result_mem[i]);
    }
  }
}
