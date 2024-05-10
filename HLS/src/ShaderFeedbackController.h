#ifndef SHADERFEEDBACKCONTROLLER_H
#define SHADERFEEDBACKCONTROLLER_H

#include "RTcore.h"

class ShaderFeedbackController
{
public:
    ShaderFeedbackController(){
      iter = 0;
    }
    
#pragma hls_design interface
#pragma hls_pipeline_init_interval 1
    void CCS_BLOCK(run)(ac_channel<ray> &ray_in,
             ac_channel<ray> &ray_scattered,
             ac_channel<buffer_obj_count> &params_in,
             ac_channel<rgb_in>& color_chan_in,
             ac_channel<rgb_in>& atten_chan_in,
             ac_channel<ray> &ray_out,
             ac_channel<buffer_obj_count> &params_out,
             ac_channel<rgb_in>& color_chan_out,
             ac_channel<rgb_in>& atten_chan_out,
             ac_channel<rgb_in>& output_pxl_serial
            )
    {

      buffer_obj_count tmp_params = params_in.read();
      rgb_in tmp_col_in = {0,0,0};
      rgb_in tmp_atten_in = {0,0,0};
      ray tmp_ray_in = {{0,0,0},{0,0,0},false};
      ray tmp_camera_ray = {{0,0,0},{0,0,0},false};

      // if its not the very first iteration of the first sample its safe to read
      // so if its not the first iteration of the first sampe
      // or if its just not the first sample-- it is safe to read channel
      if( ((iter != 0) && (tmp_params.firstsamp == true)) || (tmp_params.firstsamp == false)){
        tmp_col_in = color_chan_in.read(); // read in color
        // if its either the first iteration or last special case, we can alwasy write out to chan now
        if ((iter == 0) || ((tmp_params.lastsamp == true) && (iter == 8))){
          output_pxl_serial.write(tmp_col_in);
          #ifndef __SYNTHESIS__
          sample += 1;
          std::cout << "wrote sample: " << sample << std::endl;
          #endif
        }
      }

      // these will always be safe to read from and set to under these conditions
      // if it was first pixel first sample first bounce, it will handle the case and set
      // tmp_col_in to 0 accumulated color and on any iterations of 0
      if (iter == 0){
        // only read input ray 1x
        tmp_ray_in = ray_in.read();
        tmp_col_in = shader1_color;
        tmp_atten_in = shader1_atten;
      }

      // will only equal 8 on the very last iteration of the last pixel sample
      // but essentially, if its not the
      if (iter != 0) {
        // will make sure shader 1 gets it's old output 
        // tmp col in already set so dont need to set it again
        tmp_ray_in = ray_scattered.read();
        tmp_atten_in = atten_chan_in.read();
      }

      atten_chan_out.write(tmp_atten_in);
      color_chan_out.write(tmp_col_in);
      ray_out.write(tmp_ray_in);

      iter += 1;
      if ((tmp_params.lastsamp == false) && (iter == 8)) iter = 0;

      if ((tmp_params.lastsamp == true) && (iter == 9)) iter = 0;

      params_out.write(tmp_params);
  }
private:
  const rgb_in shader1_atten = {1, 1, 1};
  const rgb_in shader1_color = {0, 0, 0};
  ac_int<4, false> iter;
  int sample = 0;
};

#endif

