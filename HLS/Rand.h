#ifndef _GLOBAL_SIMPLE_H
#define _GLOBAL_SIMPLE_H

#include <ac_int.h>
#include <ac_fixed.h>
#include <ac_channel.h>
#include <sstream>


#include "RTcore.h"
#include "rt_core.h"
#include "shader.h"

// Include mc_scverify.h for CCS_* macros
#include <mc_scverify.h>
// TO DO: TEMPLATE IT FOR DIFFERENT SEEDS
/// NEED SOME HANDSHAKE PROTOCOL TO DICTATE WHEN TO FETCH NEXT RAND NUM
class Rand
{
  public:
    Rand(uint_22 seed = 11301){state(seed)} // on reset these will be the values
    
    #pragma hls_design interface
    void CCS_BLOCK(run)(ac_channel<uint_22> &din, // do i need to make it so i can fetch on command
                        ac_channel<sfp_3_22> &output_num)
    {
      uint_22 new_bit = ((state >> 21) ^ (state >> 20)) & 1;  // Taps at positions 22 and 21
      state = ((state << 1) | new_bit) & 0x3FFFFF;  // Keep state to 22 bits
      output_num.write(state);
    }

  private:

    uint_22 state;
};

#endif


