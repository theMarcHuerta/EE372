//
// Copyright 2018 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
//

#ifndef VECTOR_MULT_H_
#define VECTOR_MULT_H_

#include <ac_int.h>
#include <ac_complex.h>
#include <ac_channel.h>

const unsigned OP1_WIDTH   = 7;
const unsigned OP2_WIDTH   = 9;
const unsigned VECTOR_SIZE = 64;

typedef ac_int<OP1_WIDTH,true>  OP1_TYPE;
typedef ac_int<OP2_WIDTH,true>  OP2_TYPE;
typedef OP1_TYPE::rt_T<OP2_TYPE>::mult  RESULT_TYPE;

void vector_mult(
  ac_channel<OP1_TYPE> &,
  ac_channel<OP2_TYPE> &,
  ac_channel<RESULT_TYPE> &
);

#endif /* VECTOR_MULT_H_ */
