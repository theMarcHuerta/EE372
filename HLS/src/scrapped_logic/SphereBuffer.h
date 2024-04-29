#ifndef SPHEREBUFFER_H
#define SPHEREBUFFER_H

#include "RTcore.h"

template <int size>
class SphBuffer {
public:
    SphBuffer(){}

    #pragma hls_design interface
    void CCS_BLOCK(run)(ac_channel<sphere_hittable> &spheres_in, 
                        ac_channel<buffer_params> &paramsIn,
                        ac_channel<sphere_hittable> &spheres_out)
    {
        // Reading buffer parameters
        buffer_params params = paramsIn.read();
        
        // Temporary storage for spheres
        chanStruct<sphere_hittable, size> buffer;
        
        // Writing to buffer
        for (int i = 0; i < params.num_spheres; i++) {
            buffer.data[i] = spheres_in.read();
        }

        // Reading from buffer multiple times based on image parameters
        for (int fy = 0; fy < params.image_height; fy++) {
            for (int fx = 0; fx < params.image_width; fx++) {
                for (int samps = 0; samps < params.samp_per_pxl; samps++) {
                    for (int i = 0; i < params.num_spheres; i++) {
                        spheres_out.write(buffer.data[i]);
                    }
                }
            }
        }
    }
};

#endif


// #include "RTcore.h"

// template <int size>
// class SphBufferWriter{
// public:
//     SphBufferWriter(){}

//     #pragma hls_design interface
//     void CCS_BLOCK(write)(ac_channel<sphere_hittable> &sphere_to_write, 
//                       ac_channel<buffer_params> &sph_buffer_params,
//                       ac_channel<chanStruct<sphere_hittable, size>> &sphere_buffer)
//     {
//         ac_channel<chanStruct<sphere_hittable, size>> mem;
//         buffer_params params;
//         params = sph_buffer_params.read();

//         for (int i = 0; i < params.num_spheres; i++) {
//             sphere_hittable sphere_streamed_in;
//             sphere_streamed_in = sphere_to_write.read();
//             mem.data[i] = sphere_streamed_in;
//         }
//         spheres_write_out.write(mem);
//     }

// };

// template <int size>
// class SphBufferReader{
// public:
//     SphBufferReader(){}

//     #pragma hls_design interface
//     void CCS_BLOCK(run)(ac_channel<Params> &paramsIn,
//                         ac_channel<chanStruct<sphere_hittable, size>> &sphere_buffer, 
//                         ac_channel<sphere_hittable> &sphere_read)
//     {
//         ac_channel<chanStruct<sphere_hittable, size>> mem;
//         buffer_params params = sph_buffer_params.read();

//         for (int fy = 0; fy < tmp_params.image_height; fy++){
//             for (int fx = 0; fx < tmp_params.image_width; fx++){
//                 for (int samps = 0; samps < tmp_params.samp_per_pxl; samps++){
//                     // for each sample we do we're gunna read the entire buffer
//                     ac_channel<chanStruct<sphere_hittable, size>> mem = sphere_buffer.read();
//                     for (int i = 0; i < params.num_spheres; i++) {
//                         sphere_hittable sphere_requested;
//                         sphere_requested = mem.data[i];
//                         dout.write(sphere_requested);
//                     }
//                 }
//             }
//         }
//     }
// };

// template <int size>
// class SphBuffer{
// public:
//   SphBuffer(){}

//   #pragma hls_design interface
//   void CCS_BLOCK(run)(ac_channel<sphere_hittable> &sphere_serial,
//                       ac_channel<buffer_params> &sph_buffer_params,
//                       ac_channel<sphere_hittable> &spheres_out)
//     {

//         buffer_params params = sph_buffer_params.read();

//         bufferReaderParams.write(params);
//         bufferWriterParams.write(params);

//         inputDoubleBufferWriter.run(sphere_serial, bufferWriterParams, mem);

//         inputDoubleBufferReader.run(bufferReaderParams, mem, spheres_out);
//     }

// private:
//     ac_channel<chanStruct<sphere_hittable, size>> mem;
    
//     SphBufferWriter<size, IC0, OC0> sphBufferWriter;
//     ac_channel<Params> bufferWriterParams;
    
//     SphBufferReader<size, IC0, OC0> sphBufferReader;
//     ac_channel<Params> bufferReaderParams;
// };

// #endif