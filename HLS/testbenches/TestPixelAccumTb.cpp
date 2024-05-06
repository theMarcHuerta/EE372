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

#include "TestPixelAccum.cpp"
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
    cam.samples_per_pixel = 3;
    cam.max_depth         = 1;
    cam.background        = cpp_vec3(0,0,0);

    cam.vfov     = 40;
    cam.lookfrom = point3(278, 278, -800);
    cam.lookat   = point3(278, 278, 0);
    cam.vup      = cpp_vec3(0,1,0);
    cam.initialize();

    // vectors of random rgb_in values to use in both cpp test and hls test
    int spp = 1024;
    int num_tests = 50000;
    std::vector<std::vector<rgb_in>> samples(num_tests);

    // generate random values to accumulate
    cout << "Generating RGB samples" << endl;
    for (int accum_tests = 0; accum_tests < num_tests; accum_tests++) {
        // create random sample vales for both cpp and hls design
        for (int i = 0; i < spp; i++) {
            double r_d = random_double();
            double g_d = random_double();
            double b_d = random_double();

            rgb_in sample;
            sample.r = r_d;
            sample.g = g_d;
            sample.b = b_d;

            samples[accum_tests].push_back(sample);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////           Run C++ model         ///////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    printf("Running C++ design\n");

    std::vector<cpp_vec3> cpp_results;

    // accumulate samples
    for (int i = 0; i < num_tests; i++) {

        cpp_vec3 accumulated_color;
        for (int j = 0; j < spp; j++) {
            accumulated_color.e[0] += samples[i][j].r.to_double();
            accumulated_color.e[1] += samples[i][j].g.to_double();
            accumulated_color.e[2] += samples[i][j].b.to_double();
        }

        cpp_results.push_back(scale_and_clamp_color(accumulated_color, spp));
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////           Run HLS model         ///////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    TestPixelAccum inst;

    // create img_params for HLS design
    img_params params_in;

    params_in.num_quads = 5;
    params_in.samp_per_pxl = cam.samples_per_pixel;
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

    ac_channel<img_params> params_in_channel;

    ac_channel<rgb_in> rgb_sample_channel;    

    // output channels
    ac_channel<rgb_out> rgb_out_channel;

    printf("Running HLS design\n");
    // populate input channels
    for (int i = 0; i < num_tests; i++) {
        params_in_channel.write(params_in);
        for (int j = 0; j < spp; j++) {
            rgb_sample_channel.write(samples[i][j]);
        }
    }

    // run the design
    inst.run(params_in_channel, rgb_sample_channel, rgb_out_channel);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////           Final Results         ///////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // uint64_t num_rays_generated = image_height*image_width;
    uint64_t mismatches = 0;

    for (int i = 0; i < num_tests; i++){
        // read in HLS output
        rgb_out hls_out = rgb_out_channel.read();
        if (std::abs(hls_out.r.to_int() - cpp_results[i].x() > 1.0) ||
            std::abs(hls_out.g.to_int() - cpp_results[i].y() > 1.0) ||
            std::abs(hls_out.b.to_int() - cpp_results[i].z() > 1.0)) {

            mismatches++;
            cout << "Mismatch at iteration " << i << endl;
        }
    }

    cout << "Test Completed with " << mismatches << " mismatches" << endl;
    cout << "Number of Tests " << num_tests << endl;
    CCS_RETURN(0);
    
}