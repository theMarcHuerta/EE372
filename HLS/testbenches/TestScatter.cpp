#ifndef TESTSCATTER_H
#define TESTSCATTER_H

#ifdef __SYNTHESIS__
    #define LABEL(x) x:
#else
    #define LABEL(x) {}
#endif

#include "../src/RTcore.h"
#include "../src/vec3.h"
#include "../src/ray.h"
#include "../src/HitRecord.h"
#include "../src/MaterialScatter.h"

#pragma hls_design top
class TestScatter {

  public:
    TestScatter() {}

    #pragma hls_design interface
    void CCS_BLOCK(run)(ac_channel<ray>& ray_in,
                 ac_channel<HitRecord>& hit_in,
                 ac_channel<rgb_in> &attenuation_chan_in,
                 ac_channel<rgb_in> &accumalated_color_chan_in,
                 ac_channel<bool> &isHit,
                 ac_channel<rgb_in> &attenuation_chan_out,
                 ac_channel<rgb_in> &accumalated_color_out,
                 ac_channel<ray> &ray_out) {

        ray_in1.write(ray_in.read());          
        hit_in1.write(hit_in.read());
        attenuation_chan_in1.write(attenuation_chan_in.read());
        accumalated_color_chan_in1.write(accumalated_color_chan_in.read());
        isHit1.write(isHit.read());

        scatterlamb.scatter(ray_in1, hit_in1, attenuation_chan_in1, accumalated_color_chan_in1, isHit1, 
                            attenuation_chan_outl1, accumalated_color_out1, ray_out1);

        attenuation_chan_out.write(attenuation_chan_outl1.read());
        accumalated_color_out.write(accumalated_color_out1.read());
        ray_out.write(ray_out1.read());

    } 

  private:
    MaterialScatter scatterlamb;
    ac_channel<ray> ray_in1;
    ac_channel<HitRecord> hit_in1;
    ac_channel<rgb_in> attenuation_chan_in1;
    ac_channel<rgb_in> accumalated_color_chan_in1;
    ac_channel<bool> isHit1;
    ac_channel<rgb_in> attenuation_chan_outl1;
    ac_channel<rgb_in> accumalated_color_out1;
    ac_channel<ray> ray_out1;

};

#endif