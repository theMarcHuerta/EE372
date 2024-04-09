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
#include "mem_iface.h"
#include "stdlib.h"
#include "mc_scverify.h"
#define uniform(RAND_MAX) (rand()/((RAND_MAX) + 1.0))

CCS_MAIN(int argc, char **argv)
{
  cout << __FILE__ << ":Memory Interfacing Testbench" << endl;
  OP1_TYPE       op1[VECTOR_SIZE];
  OP2_TYPE       op2[VECTOR_SIZE];
  RESULT_TYPE    result[VECTOR_SIZE];
  const unsigned VECTOR_COUNT = 3;  // number of vectors to test
  unsigned       error_count = 0;
  ac_sync        run, complete;

  for (unsigned pass=0; pass<VECTOR_COUNT; pass++) {
    // fill op1 and op2 arrays with random data:
    for (unsigned addr=0; addr<VECTOR_SIZE; addr++) {
      op1[addr] = (unsigned) (uniform(1<<OP1_WIDTH));
      op2[addr] = (unsigned) (uniform(1<<OP2_WIDTH));
    }

    run.sync_out();
    CCS_DESIGN(mem_iface) (run, op1, op2, result, complete);
    complete.sync_in();

    // check the results:
    cout << __FILE__ <<
         ":Checking " << VECTOR_SIZE << " output values..." <<
         endl;
    for (unsigned addr=0; addr<VECTOR_SIZE; addr++) {
      if (result[addr] != op1[addr] + op2[addr]) {
        error_count++;
        cout << __FILE__ <<
             ":Result mismatch running pass=" <<  pass <<
             ", op1[" << addr << "]=" << op1[addr] <<
             ", op2[" << addr << "]=" << op2[addr] <<
             ", result[" << addr << "]=" << result[addr] <<
             endl;
      }
    }
  }

  cout << __FILE__ <<
       ":Testbench Complete with " << error_count <<
       " mismatches, after running " << VECTOR_COUNT << " pass(es)." <<
       endl;
  CCS_RETURN(0);
}
