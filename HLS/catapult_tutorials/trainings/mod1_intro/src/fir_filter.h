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

// this example implements a FIR filte
// refer to Wikipedia entry on FIR filter

const unsigned N = 62;          // filter order
const unsigned TAP_COUNT = N+1;  // there are N+1 coefficients for an N-order filter

void fir_filter(const double, double [], double &);

#endif /* FIR_FILTER_H_ */

