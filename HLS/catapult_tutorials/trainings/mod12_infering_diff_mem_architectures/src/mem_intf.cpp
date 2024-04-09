//
// Copyright 2018 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
//

#include "mem_intf.h"

#pragma hls_design
void mem_intf ( ac_sync     &run,
                OP1_TYPE    op1[VECTOR_SIZE],
                OP2_TYPE    op2[VECTOR_SIZE],
                RESULT_TYPE result[VECTOR_SIZE],
                ac_sync     &complete)
{

  if (run.sync_in()) {
    VECTOR_ADD:for (unsigned addr = 0; addr<VECTOR_SIZE; addr++) {
      result[addr] = op1[addr] + op2[addr];
    }
    complete.sync_out();
  }
}
