#ifndef _INCLUDED_AC_SQRT_PWL_TB_H_
#define _INCLUDED_AC_SQRT_PWL_TB_H_

#include <ac_float.h>
#include <ac_fixed.h>
#include <ac_complex.h>

// Include specific header file
#include <ac_math/ac_sqrt_pwl.h>

//#define TEST_SQRT_AC_FIXED


//Declaration of ac_fixed datatypes
#if defined(TEST_SQRT_AC_FIXED)
typedef ac_fixed<32,16, false, AC_RND, AC_SAT> input_type;
typedef ac_fixed<32,16, false, AC_RND, AC_SAT> output_type;
#else

//Declaration of ac_float datatypes
#if defined(TEST_SQRT_AC_FLOAT)
typedef ac_float<9, 3, 1, AC_RND> input_type;
typedef ac_float<32, 16, 4, AC_RND> output_type;
//typedef ac_float<5, 1, 1, AC_RND> output_type;
#else

//Declaration of ac_complex <ac_fixed> datatypes
#if defined(TEST_SQRT_AC_COMPLEX_AC_FIXED)
typedef ac_complex<ac_fixed<32, 12, true, AC_RND, AC_SAT> > input_type;
typedef ac_complex<ac_fixed<32, 16, true, AC_RND, AC_SAT> > output_type;
#else
#error Must select a datatype
#endif
#endif
#endif

// Function prototype
void project(
    input_type &input,
    output_type &output
);

#endif
