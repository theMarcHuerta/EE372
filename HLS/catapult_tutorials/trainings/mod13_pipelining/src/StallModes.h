//
// Copyright 2018 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
//

#ifndef STALLMODES_H_
#define STALLMODES_H_

#include <ac_int.h>
#include <ac_fixed.h>
#include <ac_channel.h>

const unsigned INPUT_WIDTH  = 20;
const unsigned INPUT_INT    = 10;
const unsigned OUTPUT_WIDTH = 10;
const unsigned OUTPUT_INT   = 5;

typedef ac_fixed<INPUT_WIDTH,INPUT_INT,false>  INPUT_TYPE;
typedef ac_fixed<OUTPUT_WIDTH,OUTPUT_INT,false,AC_RND_CONV_ODD,AC_SAT> OUTPUT_TYPE;

void StallModes (
  ac_channel<INPUT_TYPE> &,
  ac_channel<OUTPUT_TYPE> &
);

#endif /* STALLMODES_H_ */
