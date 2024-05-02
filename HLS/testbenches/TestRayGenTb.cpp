#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <ac_channel.h>
#include <ac_int.h>
#include <ac_fixed.h>
#include <ac_math.h>
#include <ac_channel.h>

#include "TestRayGen.cpp"
#include "../src/path_tracer_gold_model/camera.h"
#include "read_stimulus.cpp"

#define LONGEST_DISTANCE 2896


CCS_MAIN(int argc, char** argv) {

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////           C++ gold model param instantiation          ////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    std::vector<shared_ptr<quad>> world;

    auto red   = (cpp_vec3(.65, .05, .05));
    auto white = (cpp_vec3(.73, .73, .73));
    auto green = (cpp_vec3(.12, .45, .15));
    auto light = (cpp_vec3(15, 15, 15));
    auto blue = (cpp_vec3(.05, .05, .65));

    world.push_back(make_shared<quad>(point3(555,0,0), cpp_vec3(0,555,0), cpp_vec3(0,0,555), 0, false, green));
    world.push_back(make_shared<quad>(point3(0,0,0), cpp_vec3(0,555,0), cpp_vec3(0,0,555), 0, false, red));
    world.push_back(make_shared<quad>(point3(343, 554, 332), cpp_vec3(-130,0,0), cpp_vec3(0,0,-105), 1, false, light));
    world.push_back(make_shared<quad>(point3(0,0,0), cpp_vec3(555,0,0), cpp_vec3(0,0,555), 0, false, white));
    world.push_back(make_shared<quad>(point3(555,555,555), cpp_vec3(-555,0,0), cpp_vec3(0,0,-555), 0, false, white));
    world.push_back(make_shared<quad>(point3(0,0,555), cpp_vec3(555,0,0), cpp_vec3(0,555,0), 0, false, white));
    //invisible wall
    world.push_back(make_shared<quad>(point3(0, 0, 0), cpp_vec3(0,555,0), cpp_vec3(555,0,0), 0, true, blue));
    box(point3(265,0,275), point3(430,330,420), 0, white, 14, world);
    box(point3(105,0,85), point3(260,165,235), 0, white, -18, world);

    int image_height = 240;
    int image_width = 240;

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = image_height;
    cam.samples_per_pixel = 1;
    cam.max_depth         = 1;
    cam.background        = cpp_vec3(0,0,0);

    cam.vfov     = 40;
    cam.lookfrom = point3(278, 278, -800);
    cam.lookat   = point3(278, 278, 0);
    cam.vup      = cpp_vec3(0,1,0);
    cam.initialize();

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////           Run C++ model         ///////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    printf("Running C++ design\n");
    std::vector<c_ray> cpp_rays;

     for (int i = 0; i < image_height; i++) {
        for (int j = 0; j < image_width; j++) {
            cpp_rays.push_back(cam.get_ray(j, i));
        }
     }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////           Run HLS model         ///////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    TestRayGen inst;

    // create img_params for HLS design
    img_params params_in;

    params_in.num_quads = 5;
    params_in.samp_per_pxl = 2;
    params_in.background = {0.0, 0.0, 0.0};
    params_in.image_height = image_height;
    params_in.image_width = image_width;

    params_in.center.x = cam.lookfrom.x();
    params_in.center.y = cam.lookfrom.y();
    params_in.center.z = cam.lookfrom.z();

    params_in.pixel00_loc.x = cam.pixel00_loc.x();
    params_in.pixel00_loc.y = cam.pixel00_loc.y();
    params_in.pixel00_loc.z = cam.pixel00_loc.z();

    params_in.pixel_delta_u.x = cam.pixel_delta_u.x();
    params_in.pixel_delta_u.y = cam.pixel_delta_u.y();
    params_in.pixel_delta_u.z = cam.pixel_delta_u.z();

    params_in.pixel_delta_v.x = cam.pixel_delta_v.x();
    params_in.pixel_delta_v.y = cam.pixel_delta_v.y();
    params_in.pixel_delta_v.z = cam.pixel_delta_v.z();

    // input channels
    ac_channel<LoopIndices> loop_indices_in;
    ac_channel<img_params> params_in_channel;    

    // output channels
    ac_channel<buffer_obj_count> paramsOut;
    ac_channel<ray> rayOut;

    printf("Running HLS design\n");
    // populate input channels
    for (int i = 0; i < image_height; i++) {
        for (int j = 0; j < image_width; j++) {
            params_in_channel.write(params_in);

            LoopIndices indices;
            indices.x_pxl = j;
            indices.y_pxl = i;
            indices.cur_samp = 0;
            loop_indices_in.write(indices);
        }
    }

    // run the design
    inst.run(loop_indices_in, params_in_channel, paramsOut, rayOut);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////           Final Results         ///////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    uint64_t num_rays_generated = image_height*image_width;
    uint64_t mismatches = 0;

    for (int i = 0; i < num_rays_generated; i++){
        // read in HLS output
        ray hls_out = rayOut.read();
        if (std::abs(hls_out.orig.x.to_double() - cpp_rays[i].orig.x() > 1.0) ||
            std::abs(hls_out.orig.y.to_double() - cpp_rays[i].orig.y() > 1.0) ||
            std::abs(hls_out.orig.z.to_double() - cpp_rays[i].orig.z() > 1.0) ||
            std::abs(hls_out.dir.x.to_double() - cpp_rays[i].dir.x() > 1.0) ||
            std::abs(hls_out.dir.y.to_double() - cpp_rays[i].dir.y() > 1.0) ||
            std::abs(hls_out.dir.z.to_double() - cpp_rays[i].dir.z() > 1.0)) {

            mismatches++;
            if (i % 10 == 0) {
                cout << "HLS result: " << hls_out.dir.x.to_double() << " " << hls_out.dir.y.to_double() << " " << hls_out.dir.z.to_double() << endl;
                cout << "Gold result: " << cpp_rays[i].dir.x() << " " << cpp_rays[i].dir.y() << " " << cpp_rays[i].dir.z() << endl;
            }
        }
    }

    cout << "Test Completed with " << mismatches << " mismatches" << endl;
    cout << "Number of Tests " << cpp_rays.size() << endl;
    CCS_RETURN(0);
    
}