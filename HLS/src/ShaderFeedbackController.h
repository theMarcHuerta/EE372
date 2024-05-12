#ifndef SHADERFEEDBACKCONTROLLER_H
#define SHADERFEEDBACKCONTROLLER_H

#include "RTcore.h"


class ShaderFeedbackController
{
public:
    ShaderFeedbackController(){
      iter = 0;
      sample = 0;
    }
    
#pragma hls_design interface
// #pragma hls_pipeline_init_interval 1
    void CCS_BLOCK(run)(ac_channel<ray> &ray_chan_in,
             ac_channel<ray> &ray_scattered_chan,
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
      rgb_in col_in = {0,0,0};
      col_in = color_chan_in.read();

      rgb_in atten_in = {0,0,0};
      atten_in = atten_chan_in.read();

      ray camera_ray_in = {{0,0,0},{0,0,0},false};
      camera_ray_in = ray_chan_in.read();

      ray scattered_ray_in = {{0,0,0},{0,0,0},false};
      scattered_ray_in = ray_scattered_chan.read();

      rgb_in tmp_col = {0,0,0};
      rgb_in tmp_atten = {1,1,1};
      ray tmp_ray = {{0,0,0},{0,0,0},false};

      if (iter == 0){
        if (sample != 0){
          output_pxl_serial.write(col_in);
        }
        tmp_col = shader1_color;
        tmp_atten = shader1_atten;
        tmp_ray = camera_ray_in;
        iter += 1;
      }
      else {
        tmp_col = col_in;
        tmp_atten = atten_in;
        tmp_ray = scattered_ray_in;
        iter += 1;
        if (iter == 8){
          iter = 0;
        }
      }
      // TO SHADER
      atten_chan_out.write(tmp_atten);
      color_chan_out.write(tmp_col);
      ray_out.write(tmp_ray);
      params_out.write(params_in.read());
      // COUNTER
      sample += 1;
  }
private:
  const rgb_in shader1_atten = {1, 1, 1};
  const rgb_in shader1_color = {0, 0, 0};
  ac_int<4, false> iter;
  int sample;
};

#endif


