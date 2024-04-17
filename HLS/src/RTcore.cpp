#ifndef RENDER_TOP_CPP
#define RENDER_TOP_CPP


#ifdef __SYNTHESIS__
    #define LABEL(x) x:
#else
    #define LABEL(x) {}
#endif

#include "Serializer.h"
#include "RTcore.h"
#include "Renderer.h"
#include "Deserializer.h"
#include <mc_scverify.h>

#include <ac_int.h>
#include <ac_fixed.h>
#include <ac_math.h>
#include <ac_channel.h>



#pragma hls_design top
class RT_core{
public:
    RT_core(){}

#pragma hls_design interface
    void CCS_BLOCK(run)(ac_channel<sphere_hittable> &sphere_serial, 
                        ac_channel<quad_hittable> &quad_serial, 
                        ac_channel<int_11> &img_params_in,
                        ac_channel<rgb_t> &output_pxl_serial)
    {
        // will initialize the variables and all
        // find a way to find max buffer size
        paramsDeserializer.run(img_params_in, num_spheres, num_quads, renderer_params, outputSerializerParams);

        sphereBuffer.run(sphere_serial, spheres_out, num_spheres);
        quadsBuffer.run(quad_serial, quads_out, num_quads);

        renderer.run(renderer_params, spheres_out, quads_out, output_pxl);

        outputSerializer.run(output_pxl, output_pxl_serial, outputSerializerParams);   
    }

private:
    ParamsDeserializer paramsDeserializer;
    
    Serializer<PackedInt<OUTPUT_PRECISION, ARRAY_DIMENSION>, ODTYPE, ARRAY_DIMENSION, ACCUMULATION_BUFFER_SIZE> outputSerializer;
    ac_channel<Params> outputSerializerParams;

    InputDoubleBuffer<INPUT_BUFFER_SIZE, ARRAY_DIMENSION, ARRAY_DIMENSION> inputDoubleBuffer;
    ac_channel<Params> sphere_md_buffer;

    WeightDoubleBuffer<WEIGHT_BUFFER_SIZE, ARRAY_DIMENSION, ARRAY_DIMENSION> weightDoubleBuffer;
    ac_channel<Params> quad_md_buffer;
    
    ac_channel<sphere_hittable[MAX_SPHERES]> spheres_out;
    ac_channel<quad_hittable[MAX_QUADS]> quads_out;
    ac_channel<rgb_t> output_pxl;    

    rendererWrapper renderer;
    ac_channel<img_params> renderer_params;

    ac_channel<int_11> num_spheres;
    ac_channel<int_11> num_quads;
};

#endif
