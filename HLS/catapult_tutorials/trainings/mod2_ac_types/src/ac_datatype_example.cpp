//
// Copyright 2018 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
//

#include <ac_int.h>
#include <ac_fixed.h>
#include <math/mgc_ac_math.h>

using std::endl;
using std::cout;

int main()
{
  // define some integer data types:
  ac_int<3,true>    a;    // Variable "a" is signed, and is 3 bits wide.
  ac_int<12,false>  b;    // Variable "b" is unsigned, and is 12 bits wide.
  ac_int<3217,true> c;    // Variable "c" is signed, and is 3217 bits wide.
  ac_fixed<15,8,false> two = 2.00;

  // define some fixed-point types:
  ac_fixed<8,1,false> d; // Variable "d" is unsigned, 8 bits wide,
  // of which the MSB is an integer bit
  // and the rest are fractional bits.
  // Assignments made to d will truncate
  // fractional bits as required, and will
  // wrap around if the assigned value is
  // out of range.
  // The format is u1,7

  ac_fixed<12,3,true,AC_RND,AC_SAT>  e;    // Variable "e" is signed, 12 bits wide,
  // of which the 3 MSBs are integer bits
  // and the rest are fractional bits.
  // Assignments made to e will have simple
  // rounding applied and will saturate to
  // the min or max value if the range is
  // exceeded.
  // The format is s3.9

  ac_fixed<7,7,true,AC_RND_CONV,AC_SAT_SYM>  f;
  // Variable "f" is signed, 7 bits wide,
  // of which all 7 bits are integer bits.
  // There are no fractional bits. Even though
  // there is no fractional component, this
  // may preferable to an ac_int because of
  // rounding and saturation capabilities.

  sqrt(two, d);
#ifndef __SYNTHESIS__
  cout << "d=" << d << " (simple rounding to 2**(-7))" << endl;
#endif

  e = 22.5;
#ifndef __SYNTHESIS__
  cout << "e=" << e << " (saturate)" << endl;
#endif

  e = 3.707070707;
#ifndef __SYNTHESIS__
  cout << "e=" << e << " (round)" << endl;
#endif

  f = -86.3;
#ifndef __SYNTHESIS__
  cout << "f=" << f << " (saturate to symmetric range)" << endl;
#endif

  f = 24.5;
#ifndef __SYNTHESIS__
  cout << "f=" << f << " (round to even when centered)" << endl;
#endif

  return 0;
}

