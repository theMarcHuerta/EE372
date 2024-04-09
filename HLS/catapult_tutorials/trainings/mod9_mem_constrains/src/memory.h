//
// Copyright 2018 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
//

#ifndef MEMORY_H_
#define MEMORY_H_

#include <ac_int.h>
#include <ac_channel.h>

const unsigned MEM_DATA_WIDTH    = 16;
const unsigned MEM_VECTOR_LENGTH = 512;

const unsigned MEM_OP1_ADDR   = 0;
const unsigned MEM_OP2_ADDR   = MEM_OP1_ADDR + MEM_VECTOR_LENGTH;
const unsigned MEM_OP3_ADDR   = MEM_OP2_ADDR + MEM_VECTOR_LENGTH;
const unsigned MEM_SIZE       = MEM_OP3_ADDR + MEM_VECTOR_LENGTH;
const unsigned OP_WIDTH       = 2;
const unsigned MEM_ADDR_WIDTH = ac::log2_ceil<MEM_SIZE>::val;

typedef ac_int<MEM_DATA_WIDTH,false> MEM_DATA_TYPE;
typedef ac_int<MEM_ADDR_WIDTH,false> MEM_ADDR_TYPE;
typedef ac_int<OP_WIDTH,false>       MEM_OP_TYPE;
const unsigned READ=0, WRITE=1, EXECUTE=2;

typedef struct {
  MEM_ADDR_TYPE  addr;
  MEM_DATA_TYPE  data;
  MEM_OP_TYPE    op;
} MEM_TRANS_IN_TYPE;

typedef struct {
  MEM_ADDR_TYPE  addr;
  MEM_DATA_TYPE  data;
} MEM_TRANS_OUT_TYPE;

void memory(
  ac_channel<MEM_TRANS_IN_TYPE> &,
  ac_channel<MEM_TRANS_OUT_TYPE> &
);

#endif /* MEMORY_H_ */
