#ifndef _INCLUDED_AC_SINCOS_LUT_TB_H_
#define _INCLUDED_AC_SINCOS_LUT_TB_H_

#include <ac_fixed.h>
#include <ac_complex.h>

// Configure bit-widths for a specific test
//  For the ac_fixed datatype, default configuration is ac_fixed<12,0,false> for input, ac_fixed<24,2,true> for the output.
//  Supply compile-time flags to change the bit-widths
//  g++ -DWivalue=11 -DIivalue=0 -DSivalue=true -DWoutvalue=22 -DIoutvalue=2 -DSoutvalue=true

#ifdef Wivalue
const int Wi = Wivalue;
#else
const int Wi = 12;
#endif

#ifdef Iivalue
const int Ii = Iivalue;
#else
const int Ii = 0;
#endif

#ifdef Sivalue
const bool Si = Sivalue;
#else
const bool Si = false;
#endif

#ifdef Woutvalue
const int Wout = Woutvalue;
#else
const int Wout = 24;
#endif

#ifdef Ioutvalue
const int Iout = Ioutvalue;
#else
const int Iout = 2;
#endif

#ifdef Soutvalue
const bool Sout = Soutvalue;
#else
const bool Sout = true;
#endif

// Define types for ac_fixed usage
typedef ac_fixed<Wi, Ii, Si, AC_RND, AC_SAT>          input_type;
typedef ac_fixed<Wout, Iout, Sout, AC_RND, AC_SAT>    output_ri_type;
typedef ac_complex<output_ri_type>                    output_type;

// Function prototype
void  project(const input_type     &angle,
              output_type          &output);


#endif 

