//******************************************************************************
// File: types.h
// Description
//   Defines constants and types used in design
//******************************************************************************

#ifndef _INCLUDED_TYPES_H_
#define _INCLUDED_TYPES_H_

#include <ac_channel.h>
#include <ac_fixed.h>

// Helper struct to compute bitwidths based on template parameters
template <int widthParam, int heightParam, int ksizeParam, int i_fmapParam, int o_fmapParam>
struct eyerissHelper {
  enum {
    dwidthVal = (1 << ac::log2_ceil<ac::log2_ceil<(widthParam *heightParam)>::val>::val),
    cntwidthVal = (1 << ac::log2_ceil<ac::log2_ceil<(widthParam*heightParam + i_fmapParam*ksizeParam + o_fmapParam*ksizeParam + o_fmapParam - 2)>::val>::val),
    weightrowVal = (1 << ac::log2_ceil<(o_fmapParam *ksizeParam)>::val),
    weightcolVal = (1 << ac::log2_ceil<(i_fmapParam *ksizeParam)>::val)
  };
};

// Type used for Input and Output data bit width.Increase the bit size to get cost and accuracy trade-offs
// For single channel image processing, keep the integer part to 9bit
typedef ac_fixed<16, 9, true> pxlType;

// Structure to validate the i/o data.
typedef struct {
  pxlType data;
  bool vld;
} pxlvldType;

template <int KSIZE, int IFMAP>
struct weightsChan {
  pxlType data[KSIZE*IFMAP];
};

// Provides N_CHANS-wide packed feature data
template <class fdType, int N_CHANS>
struct packedFData {
  fdType data[N_CHANS];
};

#endif

