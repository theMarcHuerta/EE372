//
// Copyright 2018 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
//

//Description: compute the inverse square root of the input value, then stream to output:
#include "StallModes.h"
#include <mc_scverify.h>
#include <ac_math.h>

#pragma hls_design top
void CCS_BLOCK(StallModes) ( ac_channel<INPUT_TYPE>  &x_chan,
                             ac_channel<OUTPUT_TYPE> &f_chan)
{

#ifndef __SYNTHESIS__
  while (x_chan.available(1))
#endif
  {
    INPUT_TYPE  x = x_chan.read();

    typedef ac_fixed<INPUT_TYPE::width,INPUT_TYPE::i_width,false,AC_RND,AC_SAT>  INV_TYPE;

    INV_TYPE  x_inv;
    ac_fixed<1,1,false>  unity = 1.0;

    OUTPUT_TYPE  f;

    // handle boundary condition:
    if (x==0.0) {
      f.set_val<AC_VAL_MAX>();
    } else {
      ac_math::ac_div(unity,x,x_inv);
      ac_math::ac_sqrt(x_inv,f);
    }

    f_chan.write(f);
  }
}
