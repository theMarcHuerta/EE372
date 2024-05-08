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

#include "TestQuadBuffer.cpp"
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

    int image_height = 30;
    int image_width = 30;

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = image_height;
    cam.samples_per_pixel = 0;
    cam.max_depth         = 1;
    cam.background        = cpp_vec3(0,0,0);

    cam.vfov     = 40;
    cam.lookfrom = point3(278, 278, -800);
    cam.lookat   = point3(278, 278, 0);
    cam.vup      = cpp_vec3(0,1,0);
    cam.initialize();

    int spp = (cam.samples_per_pixel == 0) ? 32 :
              (cam.samples_per_pixel == 1) ? 64 :
              (cam.samples_per_pixel == 2) ? 256 : 1024;

    // vector of hls quad hittables to compare with output of buffer
    std::vector<quad_hittable> quads;

    // ac channel of quads to be used as HLS input
    ac_channel<quad_hittable> quad_chan_in;

    // populate quad hittable vector with quads added to world in previous section
    for (int i = 0; i < world.size(); i++) {
        quad_hittable q;

        q.corner_pt.x = world[i]->Q.x();
        q.corner_pt.y = world[i]->Q.y();
        q.corner_pt.z = world[i]->Q.z();

        q.u.x = world[i]->u.x();
        q.u.y = world[i]->u.y();
        q.u.z = world[i]->u.z();

        q.v.x = world[i]->v.x();
        q.v.y = world[i]->v.y();
        q.v.z = world[i]->v.z();

        q.mat_type = world[i]->mat;
        q.is_invis = world[i]->invis;

        q.normal.x = world[i]->normal.x();
        q.normal.y = world[i]->normal.y();
        q.normal.z = world[i]->normal.z();

        q.w.x = world[i]->w.x();
        q.w.y = world[i]->w.y();
        q.w.z = world[i]->w.z();

        q.d_plane = world[i]->D;

        q.quad_color.r = world[i]->obj_color.x();        
        q.quad_color.g = world[i]->obj_color.y();
        q.quad_color.b = world[i]->obj_color.z();

        // add to cpp result vector
        quads.push_back(q);

        // add to hls input channel
        quad_chan_in.write(q);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////           Run HLS model         ///////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    TestQuadBuffer inst;

    // create img_params for HLS design
    buffer_params params_in;

    params_in.num_quads = quads.size();
    params_in.samp_per_pxl = cam.samples_per_pixel;
    params_in.image_height = image_height;
    params_in.image_width = image_width;

    ac_channel<buffer_params> params_in_channel;
    params_in_channel.write(params_in);

    // output quad channel
    ac_channel<quad_hittable> quads_out;

    printf("Running HLS design\n");

    // run the design
    inst.run(quad_chan_in, params_in_channel, quads_out);

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

    ac_int<22, false> pixels_in_img = (params_in.image_height) * (params_in.image_width);
    ac_int<33, false> tot_samples_in_img = pixels_in_img * (spp);
    ac_int<36, false> tot_read_requests = (tot_samples_in_img << 3) + 1; // multiply by 8 plus 1 for the extra end case bounce

    for (int i = 0; i < tot_read_requests; i++) {
        for (int j = 0; j < quads.size(); j++) {
            quad_hittable q_out = quads_out.read();

            // compare all quad data
            if ((q_out.corner_pt.x != quads[j].corner_pt.x) ||
                (q_out.corner_pt.y != quads[j].corner_pt.y) ||
                (q_out.corner_pt.z != quads[j].corner_pt.z) ||
                (q_out.u.x != quads[j].u.x) ||
                (q_out.u.y != quads[j].u.y) ||
                (q_out.u.z != quads[j].u.z) ||
                (q_out.v.x != quads[j].v.x) ||
                (q_out.v.y != quads[j].v.y) ||
                (q_out.v.z != quads[j].v.z) ||
                (q_out.mat_type != quads[j].mat_type) ||
                (q_out.is_invis != quads[j].is_invis) ||
                (q_out.normal.x != quads[j].normal.x) ||
                (q_out.normal.y != quads[j].normal.y) ||
                (q_out.normal.z != quads[j].normal.z) ||
                (q_out.w.x != quads[j].w.x) ||
                (q_out.w.y != quads[j].w.y) ||
                (q_out.w.z != quads[j].w.z) ||
                (q_out.d_plane != quads[j].d_plane) ||
                (q_out.quad_color.r != quads[j].quad_color.r) ||
                (q_out.quad_color.g != quads[j].quad_color.g) ||
                (q_out.quad_color.b != quads[j].quad_color.b)) {
                    mismatches++;
            }
        }
    }

    cout << "Test Completed with " << mismatches << " mismatches" << endl;
    CCS_RETURN(0);
    
}