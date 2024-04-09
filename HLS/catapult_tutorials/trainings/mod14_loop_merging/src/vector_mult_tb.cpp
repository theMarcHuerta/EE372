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
#include "vector_mult.h"
#include "mc_scverify.h"
#include "stdlib.h"
#include <math/mgc_ac_math.h>
#define uniform(RAND_MAX) (rand()/((RAND_MAX) + 1.0))

CCS_MAIN(int argc, char **argv)
{
  cout << __FILE__ << ":Loop Constraints Testbench" << endl;
  OP1_TYPE    op1[VECTOR_SIZE];
  OP2_TYPE    op2[VECTOR_SIZE];
  RESULT_TYPE result[VECTOR_SIZE];

  unsigned       error_count = 0;
  const unsigned PASS_COUNT = 1;

  ac_channel<OP1_TYPE>    op1_chan;
  ac_channel<OP2_TYPE>    op2_chan;
  ac_channel<RESULT_TYPE> result_chan;

  for (unsigned i=0; i<PASS_COUNT; i++) {

    // fill op1 and op2 arrays with random data:
    for (unsigned addr=0; addr<VECTOR_SIZE; addr++) {
      // randomize op1:
      op1[addr] = (unsigned) (uniform(1<<OP1_TYPE::width));
      op1_chan.write(op1[addr]);
      // randomize op2:
      op2[addr] = (unsigned) (uniform(1<<OP1_TYPE::width));
      op2_chan.write(op2[addr]);

      cout << __FILE__ << "," << __LINE__ << ":" <<
           "op1[" << addr << "]=" << op1[addr] << ", " <<
           "op2[" << addr << "]=" << op2[addr] << endl;
    }

    // call dut:
    vector_mult(op1_chan, op2_chan, result_chan);

    // check the results:
    cout << __FILE__ << "," << __LINE__ << ":Checking " <<
         VECTOR_SIZE << " output values..." << endl;
    for (unsigned addr=0; addr<VECTOR_SIZE; addr++) {
      result_chan.read(result[addr]);
      RESULT_TYPE  exp_result = op1[addr] * op2[addr];

      cout << __FILE__ << ":Results: " <<
           "op1[" << addr << "]=" << op1[addr] <<
           ", op2[" << addr << "]=" << op2[addr] <<
           ", result[" << addr << "]=" << result[addr];

      if (result[addr] != exp_result) {
        error_count++;
        cout << ", Error, expected value is " << exp_result;
      }
      cout << endl;
    }
  }

  cout << __FILE__ << "," << __LINE__ <<
       ":Testbench Complete after processing " << PASS_COUNT <<
       " packets with " << error_count << " mismatches." << endl;
  CCS_RETURN(0);
}
