////////////////////////////////////////////////////////////////////////////////
//  Source:         sobel.h
//  Description:    Type definitions for ac_window_2d example Sobel edge detect
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDED_SOBEL_H_
#define _INCLUDED_SOBEL_H_

#include <ac_int.h>

template <class T>
struct dataWithFlags {
  T               data;
  ac_int<1,false> sof;
  ac_int<1,false> eof;
  ac_int<1,false> sol;
  ac_int<1,false> eol;
};

#endif

