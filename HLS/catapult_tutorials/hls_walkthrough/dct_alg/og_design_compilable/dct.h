//
// Copyright 2003-2015 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
// 
#ifndef DCT_H_
#define DCT_H_
#include <ac_channel.h>
#include <ac_int.h>
#include "jpeg.h"

void dct(char *input[8], short *output[8]);
#endif /* DCT_H_ */
