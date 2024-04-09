//
// Copyright 2018 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
//

#ifndef DECIMATOR_H_
#define DECIMATOR_H_

#include <ac_int.h>
#include <ac_fixed.h>
#include <ac_channel.h>

const unsigned RATE_BITS         = 3;
const unsigned MAX_RATE          = 1<<RATE_BITS;
typedef  ac_fixed<9,7,true>      IO_TYPE;
typedef  ac_int<RATE_BITS,false> RATE_TYPE;

void decimator (ac_channel<IO_TYPE> &, const RATE_TYPE, ac_channel<IO_TYPE> &);

#endif /* DECIMATOR_H_ */

