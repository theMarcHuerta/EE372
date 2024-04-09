//
// Copyright 2018 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
//

#ifndef FIR_FILTER_H_
#define FIR_FILTER_H_

#include <ac_fixed.h>

const unsigned FILTER_ORDER = 126;         // filter order
const unsigned TAP_COUNT = FILTER_ORDER+1; // there are N+1 coefficients for an N-order filter

typedef ac_fixed<3,1,true>   X_TYPE;
typedef ac_fixed<10,-2,true> COEFF_TYPE;
typedef ac_fixed<9,1,true,AC_RND,AC_SAT_SYM> Y_TYPE;

void fir_filter(const X_TYPE, COEFF_TYPE [TAP_COUNT], Y_TYPE &);

#endif /* FIR_FILTER_H_ */

