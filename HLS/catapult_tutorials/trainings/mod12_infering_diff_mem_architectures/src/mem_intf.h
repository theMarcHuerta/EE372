//
// Copyright 2018 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
//

#ifndef MEM_INTF_H_
#define MEM_INTF_H_

#include "ac_int.h"
#include "ac_sync.h"

const unsigned OP1_WIDTH    = 16;
const unsigned OP2_WIDTH    = 12;
const unsigned RESULT_WIDTH = 18;
const unsigned VECTOR_SIZE  = 256;

typedef ac_int<OP1_WIDTH,false>    OP1_TYPE;
typedef ac_int<OP2_WIDTH,false>    OP2_TYPE;
typedef ac_int<RESULT_WIDTH,false> RESULT_TYPE;

void mem_intf (
  ac_sync     &run,
  OP1_TYPE    op1[VECTOR_SIZE],
  OP2_TYPE    op2[VECTOR_SIZE],
  RESULT_TYPE result[VECTOR_SIZE],
  ac_sync     &complete
);

#endif /* MEM_INTF_H_ */
