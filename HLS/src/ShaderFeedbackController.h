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
    void run(ac_channel<ray> &ray_in,
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
    // #ifndef __SYNTHESIS__
    // while(params_in.available(1))
    // #endif
    // {
      #ifndef __SYNTHESIS__
      std::cout << "in feedback controller " << iter << std::endl;
      #endif

      buffer_obj_count tmp_params = params_in.read();

      // if its not the first sample it safe to read the previous scattered ray
      if ((iter == 0) && (tmp_params.firstsamp == false)){
        #ifndef __SYNTHESIS__
        pixs+=1;
        // if (pixs % 768 == 0){
          std::cout << "Sample Finished" << pixs << std::endl;
        // }
        #endif
        output_pxl_serial.write(color_chan_in.read());
      }

      // if its the last sample write out
      if ((tmp_params.lastsamp == true) && (iter == 8)) {
        output_pxl_serial.write(color_chan_in.read());
        atten_chan_in.read();
        color_chan_in.read();
        // waste a cycle here at the very end
        ray_out.write(ray_in.read());
        atten_chan_out.write(shader1_atten);
        color_chan_out.write(shader1_color);
        #ifndef __SYNTHESIS__
        std::cout << "WROTE LAST SAMPLE OUT " << iter << std::endl;
        #endif
      }

      if (iter == 0){
        // only read input ray 1x
        #ifndef __SYNTHESIS__
        std::cout << "first bounce read in from " << iter << std::endl;
        #endif
        ray_out.write(ray_in.read());
        atten_chan_out.write(shader1_atten);
        color_chan_out.write(shader1_color);
      }

      // will only equal 8 on the very last iteration of the last pixel sample
      if ((iter != 0) && (iter != 8)) {
        // will make sure shader 1 gets it's old output 
        #ifndef __SYNTHESIS__
        std::cout << "writting scattery ray out " << iter << std::endl;
        #endif
        ray_out.write(ray_scattered.read());
        atten_chan_out.write(atten_chan_in.read());
        color_chan_out.write(color_chan_in.read());
      }

        // #ifndef __SYNTHESIS__
        // std::cout << "Passed check 1 " << iter << std::endl;
        // #endif
      iter += 1;
      if ((tmp_params.lastsamp == false) && (iter == 8)) iter = 0;

      if ((tmp_params.lastsamp == true) && (iter == 9)) iter = 0;

      params_out.write(tmp_params);
      #ifndef __SYNTHESIS__
      nums += 1;
        std::cout << "BOUNCE NUMBER " << nums << std::endl;
      #endif
  }
private:
  int pixs = 0;
  const rgb_in shader1_atten = {1, 1, 1};
  const rgb_in shader1_color = {0, 0, 0};
  ac_int<4, false> iter;
  int nums = 0;

};

#endif
