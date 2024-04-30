#ifndef QUADSBUFFER_H
#define QUADSBUFFER_H

#include "RTcore.h"

template <int size>
class QuadBuffer {
public:
    QuadBuffer(){}

    #pragma hls_design interface
    #pragma hls_pipeline_init_interval 1
    void CCS_BLOCK(run)(ac_channel<quad_hittable> &quads_in, 
                        ac_channel<buffer_params> &paramsIn,
                        ac_channel<quad_hittable> &quads_out)
    {
        // Reading buffer parameters
        buffer_params params = paramsIn.read();
        
        // Temporary storage for quads
        chanStruct<quad_hittable, size> buffer;
        
        // Writing to buffer
        for (int i = 0; i < params.num_quads; i++) {
            buffer.data[i] = quads_in.read();
        }

        // Reading from buffer multiple times based on image parameters
        for (int fy = 0; fy < params.image_height; fy++) {
            for (int fx = 0; fx < params.image_width; fx++) {
                for (int samps = 0; samps < params.samp_per_pxl; samps++) {
                    for (int i = 0; i < params.num_quads; i++) {
                        quads_out.write(buffer.data[i]);
                    }
                }
            }
        }
    }
};

#endif


// #ifndef QUADSBUFFER_H
// #define QUADSBUFFER_H


// #include "RTcore.h"

// template <int size>
// class QuadBufferWriter{
// public:
//     QuadBufferWriter(){}

//     #pragma hls_design interface
//     void CCS_BLOCK(write)(ac_channel<quad_hittable> &quad_to_write, 
//                       ac_channel<buffer_params> &quad_buffer_params,
//                       ac_channel<chanStruct<quad_hittable, size>> &quad_buffer)
//     {
//         ac_channel<chanStruct<quad_hittable, size>> mem;
//         buffer_params params;
//         params = quad_buffer_params.read();

//         for (int i = 0; i < params.num_quads; i++) {
//             quad_hittable quad_streamed_in;
//             quad_streamed_in = quad_to_write.read();
//             mem.data[i] = quad_streamed_in;
//         }
//         quads_write_out.write(mem);
//     }

// };

// template <int size>
// class quadBufferReader{
// public:
//     quadBufferReader(){}

//     #pragma hls_design interface
//     void CCS_BLOCK(run)(ac_channel<Params> &paramsIn,
//                         ac_channel<chanStruct<quad_hittable, size>> &quad_buffer, 
//                         ac_channel<quad_hittable> &quad_read)
//     {
//         ac_channel<chanStruct<quad_hittable, size>> mem;
//         buffer_params params = quad_buffer_params.read();

//         for (int fy = 0; fy < tmp_params.image_height; fy++){
//             for (int fx = 0; fx < tmp_params.image_width; fx++){
//                 for (int samps = 0; samps < tmp_params.samp_per_pxl; samps++){
//                     // for each sample we do we're gunna read the entire buffer
//                     ac_channel<chanStruct<quad_hittable, size>> mem = quad_buffer.read();
//                     for (int i = 0; i < params.num_quads; i++) {
//                         quad_hittable quad_requested;
//                         quad_requested = mem.data[i];
//                         dout.write(quad_requested);
//                     }
//                 }
//             }
//         }
//     }
// };

// template <int size>
// class quadBuffer{
// public:
//   quadBuffer(){}

//   #pragma hls_design interface
//   void CCS_BLOCK(run)(ac_channel<quad_hittable> &quad_serial,
//                       ac_channel<buffer_params> &quad_buffer_params,
//                       ac_channel<quad_hittable> &quads_out)
//     {

//         buffer_params params = quad_buffer_params.read();

//         bufferReaderParams.write(params);
//         bufferWriterParams.write(params);

//         inputDoubleBufferWriter.run(quad_serial, bufferWriterParams, mem);

//         inputDoubleBufferReader.run(bufferReaderParams, mem, quads_out);
//     }

// private:
//     ac_channel<chanStruct<quad_hittable, size>> mem;
    
//     QuadBufferWriter<size, IC0, OC0> quadBufferWriter;
//     ac_channel<Params> bufferWriterParams;
    
//     QuadBufferReader<size, IC0, OC0> quadBufferReader;
//     ac_channel<Params> bufferReaderParams;
// };

// #endif