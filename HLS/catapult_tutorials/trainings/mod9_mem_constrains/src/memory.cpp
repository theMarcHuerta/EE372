//
// Copyright 2018 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
//

#include "memory.h"
#pragma hls_design top
void memory(
  ac_channel<MEM_TRANS_IN_TYPE> &ch_in,
  ac_channel<MEM_TRANS_OUT_TYPE> &ch_out)
{

#ifndef __SYNTHESIS__
  while (ch_in.available(1))
#endif
  {
    static MEM_DATA_TYPE mem[MEM_SIZE];
    static bool          b_dummy = ac::init_array<AC_VAL_DC>(&mem[0],MEM_SIZE);

    MEM_TRANS_IN_TYPE  transaction_in = ch_in.read();
    MEM_TRANS_OUT_TYPE transaction_out;

    switch (transaction_in.op) {

    case READ:
      transaction_out.addr = transaction_in.addr;
      if (transaction_out.addr < MEM_SIZE) {
        transaction_out.data = mem[transaction_in.addr];
      } else {
        transaction_out.data.set_val<AC_VAL_DC>();
      }
      ch_out.write(transaction_out);
      break;

    case WRITE:
      if (transaction_in.addr < MEM_SIZE) {
        mem[transaction_in.addr] = transaction_in.data;
      }
      break;

    case EXECUTE:
      EXE_LOOP:for (
        unsigned op1_addr=MEM_OP1_ADDR, op2_addr=MEM_OP2_ADDR, op3_addr=MEM_OP3_ADDR, i=0;
        i<MEM_VECTOR_LENGTH;
        i++, op1_addr++, op2_addr++, op3_addr++
      ) {
        mem[op3_addr] = mem[op1_addr] + mem[op2_addr];
      }
      break;

    default:;
      break;
    }
  }
}
