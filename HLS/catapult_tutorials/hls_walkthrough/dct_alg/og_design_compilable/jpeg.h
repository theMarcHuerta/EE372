//
// Copyright 2003-2015 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
// 
#pragma once

#include "huff.h"

#include <ccs_jpgfile_tables.h>
#include <ac_fixed.h>
#include <ac_channel.h>

// ----------------------------------------------------------------------------------------------
//  Data types
// ----------------------------------------------------------------------------------------------

// Huffman Codes
// DC code is index 0, AC codes 1..63
struct codes_t {
  uint6   size;
  uint32  code;
  
  codes_t() {
    size=0;
    code=0; 
  };
  
  codes_t(uint6 s, uint32 c) {
    size=s;
    code=c;
  }
};

struct rgb_t {
  uint8 r;
  uint8 g;
  uint8 b;
};

struct block64 {
   short mem[64];
};

// ----------------------------------------------------------------------------------------------
//  Forward definitions (local functions)
// ----------------------------------------------------------------------------------------------

void pixelpipe(
      ac_channel<uint2>   &blocktype,           // input  block type, Y, Cr, Cb
      ac_channel<rgb_t>   &rgb,                 // input  block
      ac_channel<codes_t> &codes );             // output huffman codes

