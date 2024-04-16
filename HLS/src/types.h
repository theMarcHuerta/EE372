#pragma once

#include "ac_fixed.h"
#include "ac_int.h"

typedef ac_fixed<32, 8, true>  fxp_32;
typedef ac_fixed<32, 8, false> ufxp_32;
typedef ac_int<24, false> rand_type;