#ifndef RAYCOLLECTOR_H
#define RAYCOLLECTOR_H

#include "RTcore.h"

class RayCollector
{
  public:
    RayCollector(){ 
    } 
    
    #pragma hls_design interface
    void CCS_BLOCK(run)(ac_channel<ray> &rayIn,
                        ac_channel<buffer_obj_count> &paramsIn, // do i need to make it so i can fetch on command
                        ac_channel<buffer_obj_count> &paramsOut,
                        ac_channel<ray> &rayOut)
    {
      #ifndef __SYNTHESIS__
      while(paramsIn.available(2))
      #endif
      {
        buffer_obj_count ray_one_params = paramsIn.read();
        buffer_obj_count ray_two_params = paramsIn.read();

        ray ray_one = rayIn.read();
        ray ray_two = rayIn.read();

        // #pragma hls_pipeline_init_interval 2
        for (int i = 0; i < 9; i++){
            // if ((i < 8)) {
            //     paramsOut.write(ray_one_params);
            //     rayOut.write(ray_one);
            //     // paramsOut.write(ray_two_params);
            //     // rayOut.write(ray_two);
            // }
            if ((i < 8) || ((i == 8) && ray_two_params.lastsamp)) {
                paramsOut.write(ray_one_params);
                rayOut.write(ray_one);
                paramsOut.write(ray_two_params);
                rayOut.write(ray_two);
            }
        }
      }
    }

  private:

};

#endif
