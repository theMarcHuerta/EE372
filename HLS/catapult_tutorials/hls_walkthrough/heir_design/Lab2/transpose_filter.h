//
// Copyright 2003-2015 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
// 

#ifndef __TPFIL__
#define __TPFIL__
#include <ac_channel.h>
#include <ac_int.h>
#include "shift_class.h"

struct memStruct{//Struct used to pack array data for ac_channel reads/writes
  uint8 data[64][64];
};

void transpose_and_filter(ac_channel<uint8 > &din, ac_channel<uint8 > &dout);

#endif
