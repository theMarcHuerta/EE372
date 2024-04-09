#ifndef _INCLUDED_AC_RECIPROCAL_PWL_TB_H_
#define _INCLUDED_AC_RECIPROCAL_PWL_TB_H_

#include <ac_float.h>
#include <ac_fixed.h>
#include <ac_complex.h>

// The following two lines have macros which allow for better debugging of the
// testbench. The user can either use them or comment them out.
// Alternatively, the user can also define them by adding appropriate
// compiler flags in the .tcl file.
// DEBUG enables the printing of various debug statements in the testbench file.
// RECIP_DEBUG enables the printing of various debug statements in the
// ac_reciprocal_pwl.h file.

//#define DEBUG
//#define RECIP_DEBUG

// Include specific header file
#include <ac_math/ac_reciprocal_pwl.h>

//mathlib_ac_reciprocal__ac_fixed_W_IN4_I_IN9_S_INtrue_E_IN0_W_OUT64_I_OUT32_S_OUTtrue_E_OUT10_lneg256u224s32e1_00

#if defined(TEST_RECIPROCAL_AC_FIXED) || defined(TEST_RECIPROCAL_AC_COMPLEX_AC_FIXED)

#ifndef W_INT_FIXED
#define W_INT_FIXED 32
#endif

#ifndef I_INT_FIXED
#define I_INT_FIXED 16
#endif

#ifndef S_BOOL_FIXED
#define S_BOOL_FIXED true
#endif

#endif

#if defined(TEST_RECIPROCAL_AC_FLOAT) || defined(TEST_RECIPROCAL_AC_COMPLEX_AC_FLOAT)

#ifndef W_INT_FLOAT
#define W_INT_FLOAT 19
#endif

#ifndef I_INT_FLOAT
#define I_INT_FLOAT 9
#endif

#ifndef E_INT_FLOAT
#define E_INT_FLOAT 10
#endif

#endif

//These macros are defined from the appropriate version of the .tcl file.
#if defined(TEST_RECIPROCAL_AC_FIXED)
typedef ac_fixed<W_INT_FIXED, I_INT_FIXED, S_BOOL_FIXED, AC_RND, AC_SAT> input_type;
typedef ac_fixed<64, 32, S_BOOL_FIXED, AC_RND, AC_SAT> output_type;
#elif defined(TEST_RECIPROCAL_AC_COMPLEX_AC_FIXED)
typedef ac_complex<ac_fixed<W_INT_FIXED, I_INT_FIXED, S_BOOL_FIXED, AC_RND, AC_SAT> > input_type;
typedef ac_complex<ac_fixed<64, 32, true, AC_RND, AC_SAT> > output_type;
#elif defined(TEST_RECIPROCAL_AC_FLOAT)
typedef ac_float<W_INT_FLOAT, I_INT_FLOAT, E_INT_FLOAT, AC_RND> input_type;
typedef ac_float<64, 32, 10, AC_RND> output_type;
#elif defined(TEST_RECIPROCAL_AC_COMPLEX_AC_FLOAT)
typedef ac_complex<ac_float<W_INT_FLOAT, I_INT_FLOAT, E_INT_FLOAT, AC_RND> > input_type;
typedef ac_complex<ac_float<64, 32, 10, AC_RND> > output_type;
#else
#error Must select a datatype
#endif

// Function prototype
void project(
  const input_type &input,
  output_type &output
);

#endif //_INCLUDED_AC_RECIPROCAL_PWL_TB_H_
