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
// #pragma hls_pipeline_init_interval 5
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
    // #ifndef __SYNTHESIS__
    // while(params_in.available(1))
    // #endif
    // {
    // if (iter < 9){

          // ray_in.read();
          // params_in.read();
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
          //// OUT MODULE
          // output_pxl_serial.write(tmp_col);
          // TO SHADER
          atten_chan_out.write(tmp_atten);
          color_chan_out.write(tmp_col);
          ray_out.write(tmp_ray);
          params_out.write(params_in.read());
          // COUNTER
          sample += 1;
          // atten_chan_out.write(tmp_atten_in);
          // color_chan_out.write(tmp_col_in);
          // ray_out.write(tmp_ray_in);
          // params_out.write(tmp_params);

      // buffer_obj_count tmp_params = params_in.read();

      // rgb_in tmp_col_in = {0,0,0};
      // rgb_in sample_color = {0,0,0};
      // rgb_in tmp_atten_in = {0,0,0};
      // ray tmp_ray_in = {{0,0,0},{0,0,0},false};
      // ray tmp_camera_ray = {{0,0,0},{0,0,0},false};
      // // bool tmp_last;

      // if (iter == 0){
      //   // only read input ray 1x
      //   tmp_ray_in = ray_in.read();
      //   tmp_col_in = shader1_color;
      //   tmp_atten_in = shader1_atten;
      //   // read the garbage output so it doesnt get stuck
      //   ray_scattered.read();
      //   atten_chan_in.read();
      //   // read the correct color;
      //   if(tmp_params.firstsamp == false){
      //     sample_color = color_chan_in.read();

      //   }
      //   else {
      //     sample_color = shader1_color;
      //     color_chan_in.read();
      //   }
      // }
      // else {
      //   ray_in.read();
      //   // will make sure shader 1 gets it's old output 
      //   // tmp col in already set so dont need to set it again
      //   tmp_ray_in = ray_scattered.read();
      //   tmp_atten_in = atten_chan_in.read();
      // }

      // // if (iter != 8){
      //   // atten_chan_out.write(tmp_atten_in);
      //   // color_chan_out.write(tmp_col_in);
      //   // ray_out.write(tmp_ray_in);
      //   // params_out.write(tmp_params);
      //   // last.write(tmp_last);
      //         // #pragma hls_pipeline_init_interval 1
      // // for (int i = 0; i < MAX_QUADS_IN_BUFFER; i++) {
      // //   quads_out.write(quads_in.read());
      // //   if (i == (tmp_params.num_quads - 1)) {
      //     atten_chan_out.write(tmp_atten_in);
      //     color_chan_out.write(tmp_col_in);
      //     ray_out.write(tmp_ray_in);
      //     params_out.write(tmp_params);
      //     // last.write(tmp_last);
      //     if ((iter == 0 && tmp_params.firstsamp == false) || ((tmp_params.lastsamp == true) && (iter == 8))){
      //       output_pxl_serial.write(sample_color);
      //       #ifndef __SYNTHESIS__
      //       sample += 1;
      //       std::cout << "wrote sample: " << sample << std::endl;
      //       #endif
      //     }
      //     // break;
      //     // }
      // // }
      // // }

      // iter += 1;
      // if ((tmp_params.lastsamp == false) && (iter == 8)) iter = 0;

      // if ((tmp_params.lastsamp == true) && (iter == 9)) iter = 0;

    // }
    // }
    // else {
    //   std::cout<< "SKIPPED LAST SHADER WRITE" << std::endl;
    // }
  }
private:
  const rgb_in shader1_atten = {1, 1, 1};
  const rgb_in shader1_color = {0, 0, 0};
  ac_int<4, false> iter;
  int sample = 0;
};

#endif


