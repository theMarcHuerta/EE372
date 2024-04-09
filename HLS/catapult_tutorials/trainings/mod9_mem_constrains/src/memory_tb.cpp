//
// Copyright 2018 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
//

#include <iostream>
using namespace std;
#include <mc_scverify.h>
#include "memory.h"
#include <vector>

CCS_MAIN(int argc, char **argv)
{
  cout << "Memory Design Testbench" << endl;
  static ac_channel<MEM_TRANS_IN_TYPE>  mem_in_chan;
  static ac_channel<MEM_TRANS_OUT_TYPE> mem_out_chan;
  unsigned expected_chan_size = 0;

  vector<MEM_TRANS_IN_TYPE>  trans_in_vector;  // inbound transactions data structure
  vector<MEM_TRANS_OUT_TYPE> trans_out_vector; // expected outbound transactions data structure

  // set up the transactions to load the op1 vector:
  for (unsigned i=0, op1_addr=MEM_OP1_ADDR; i<MEM_VECTOR_LENGTH; i++, op1_addr++) {
    MEM_TRANS_IN_TYPE trans_in_data;
    trans_in_data.addr = op1_addr;
    trans_in_data.data = i;
    trans_in_data.op   = WRITE;
    trans_in_vector.push_back(trans_in_data);
  }
  //expected_chan_size += MEM_VECTOR_LENGTH;

  // set up the transactions to load the op2 vector:
  for (unsigned i=0, op2_addr=MEM_OP2_ADDR; i<MEM_VECTOR_LENGTH; i++, op2_addr++) {
    MEM_TRANS_IN_TYPE trans_in_data;
    trans_in_data.addr = op2_addr;
    trans_in_data.data = i*100;
    trans_in_data.op   = WRITE;
    trans_in_vector.push_back(trans_in_data);
  }
  //expected_chan_size += MEM_VECTOR_LENGTH;

  // set up the transactions to zero out the op3 vector:
  for (unsigned i=0, op3_addr=MEM_OP3_ADDR; i<MEM_VECTOR_LENGTH; i++, op3_addr++) {
    MEM_TRANS_IN_TYPE trans_in_data;
    trans_in_data.addr = op3_addr;
    trans_in_data.data = 0;
    trans_in_data.op   = WRITE;
    trans_in_vector.push_back(trans_in_data);
  }
  //expected_chan_size += MEM_VECTOR_LENGTH;

  // set up the transactions to read out op1, op2, and op3 vectors to make sure they were properly written:
  for (unsigned i=0, op1_addr=MEM_OP1_ADDR; i<MEM_VECTOR_LENGTH; i++, op1_addr++) {
    MEM_TRANS_IN_TYPE trans_in_data;
    trans_in_data.addr = op1_addr;
    trans_in_data.data.set_val<AC_VAL_DC>();
    trans_in_data.op   = READ;
    trans_in_vector.push_back(trans_in_data);
  }
  expected_chan_size += MEM_VECTOR_LENGTH;

  for (unsigned i=0, op2_addr=MEM_OP2_ADDR; i<MEM_VECTOR_LENGTH; i++, op2_addr++) {
    MEM_TRANS_IN_TYPE trans_in_data;
    trans_in_data.addr = op2_addr;
    trans_in_data.data.set_val<AC_VAL_DC>();
    trans_in_data.op   = READ;
    trans_in_vector.push_back(trans_in_data);
  }
  expected_chan_size += MEM_VECTOR_LENGTH;

  for (unsigned i=0, op3_addr=MEM_OP3_ADDR; i<MEM_VECTOR_LENGTH; i++, op3_addr++) {
    MEM_TRANS_IN_TYPE trans_in_data;
    trans_in_data.addr = op3_addr;
    trans_in_data.data.set_val<AC_VAL_DC>();
    trans_in_data.op   = READ;
    trans_in_vector.push_back(trans_in_data);
  }
  expected_chan_size += MEM_VECTOR_LENGTH;

  // set up the execute transaction:
  {
    MEM_TRANS_IN_TYPE trans_in_data;
    trans_in_data.addr.set_val<AC_VAL_DC>();
    trans_in_data.data.set_val<AC_VAL_DC>();
    trans_in_data.op = EXECUTE;
    trans_in_vector.push_back(trans_in_data);
  }

  // set up the transactions to read the op3 result after execute:
  for (unsigned i=0, op3_addr=MEM_OP3_ADDR; i<MEM_VECTOR_LENGTH; i++, op3_addr++) {
    MEM_TRANS_IN_TYPE trans_in_data;
    trans_in_data.addr = op3_addr;
    trans_in_data.data.set_val<AC_VAL_DC>();
    trans_in_data.op   = READ;
    trans_in_vector.push_back(trans_in_data);
  }
  expected_chan_size += MEM_VECTOR_LENGTH;

  // push our transaction list into the mem_in channel:
  for (unsigned i=0; i<trans_in_vector.size(); i++)
  { mem_in_chan.write(trans_in_vector[i]); }

  // call the DUT:
  CCS_DESIGN(memory) (mem_in_chan, mem_out_chan);

  // sanity checks:
  assert(0==mem_in_chan.size());
  assert(expected_chan_size==mem_out_chan.size());
  // check the op1 read values:
  for (unsigned i=0, op1_addr=MEM_OP1_ADDR; i<MEM_VECTOR_LENGTH; i++, op1_addr++) {
    MEM_TRANS_OUT_TYPE mem_out_data = mem_out_chan.read();
    assert(i==mem_out_data.data);
    assert(op1_addr==mem_out_data.addr);
    cout << __FILE__ << "," << __LINE__ << ":mem[" << mem_out_data.addr << "]=" << mem_out_data.data << endl;
  }
  // check the op2 read values:
  for (unsigned i=0, op2_addr=MEM_OP2_ADDR; i<MEM_VECTOR_LENGTH; i++, op2_addr++) {
    MEM_TRANS_OUT_TYPE mem_out_data = mem_out_chan.read();
    assert(i*100==mem_out_data.data);
    assert(op2_addr==mem_out_data.addr);
    cout << __FILE__ << "," << __LINE__ << ":mem[" << mem_out_data.addr << "]=" << mem_out_data.data << endl;
  }
  // check the op3 read values prior to execute:
  for (unsigned i=0, op3_addr=MEM_OP3_ADDR; i<MEM_VECTOR_LENGTH; i++, op3_addr++) {
    MEM_TRANS_OUT_TYPE mem_out_data = mem_out_chan.read();
    assert(0==mem_out_data.data);
    assert(op3_addr==mem_out_data.addr);
    cout << __FILE__ << "," << __LINE__ << ":mem[" << mem_out_data.addr << "]=" << mem_out_data.data << endl;
  }
  // check the op3 read values after execute:
  for (unsigned i=0, op3_addr=MEM_OP3_ADDR; i<MEM_VECTOR_LENGTH; i++, op3_addr++) {
    MEM_TRANS_OUT_TYPE mem_out_data = mem_out_chan.read();
    assert(op3_addr==mem_out_data.addr);
    assert(mem_out_data.data==i+i*100);
    cout << __FILE__ << "," << __LINE__ << ":mem[" << mem_out_data.addr << "]=" << mem_out_data.data << endl;
  }
  cout << __FILE__ << "," << __LINE__ << ":End of testbench." << endl;

  CCS_RETURN(0);
}
