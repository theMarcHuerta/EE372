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

#include "Pathtracer.cpp"
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

    int image_height = 100;
    int image_width = 100;

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

    // create gold model pixel array
    cpp_vec3** pixels_cpp = new cpp_vec3*[image_height];
    cam.render_gold(world, pixels_cpp);


    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////           Run HLS model         ///////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////
    // INITIALIZE HLS IMAGE PARAMS
    ////////////////////////////////
    img_params ip;
    ip.num_quads = quads.size();
    ip.samp_per_pxl = cam.samples_per_pixel;
    ip.background.r = cam.background.x();
    ip.background.g = cam.background.y();
    ip.background.b = cam.background.z();
    ip.image_height = image_height;
    ip.image_width = image_width;
    ip.center.x = cam.lookfrom.x();
    ip.center.y = cam.lookfrom.y();
    ip.center.z = cam.lookfrom.z();
    ip.pixel00_loc.x = cam.pixel00_loc.x();
    ip.pixel00_loc.y = cam.pixel00_loc.y();
    ip.pixel00_loc.z = cam.pixel00_loc.z();
    ip.pixel_delta_u.x = cam.pixel_delta_u.x();
    ip.pixel_delta_u.y = cam.pixel_delta_u.y();
    ip.pixel_delta_u.z = cam.pixel_delta_u.z();
    ip.pixel_delta_v.x = cam.pixel_delta_v.x();
    ip.pixel_delta_v.y = cam.pixel_delta_v.y();
    ip.pixel_delta_v.z = cam.pixel_delta_v.z();

    ////////////////////////////////
    // INITIALIZE HLS BUFFER PARAMS
    ////////////////////////////////
    buffer_params buf_params;
    buf_params.num_quads = quads.size();
    buf_params.samp_per_pxl = cam.samples_per_pixel;
    buf_params.image_height = image_height;
    buf_params.image_width = image_width;

    ////////////////////////////////
    // POPULATE INPUT CHANNEL
    ////////////////////////////////
    ac_channel<ac_int<12,false>> in;
    ac_int<12, false> x;

    x = 0;
    x.set_slc(0, ip.num_quads.slc<11>(0));
    in.write(x);

    x = ip.samp_per_pxl;
    in.write(x);

    // RED COLOR
    x = 0;
    x.set_slc(0, ip.background.r.slc<12>(0));
    in.write(x);

    x = 0;
    x.set_slc(0, ip.background.r.slc<12>(12));
    in.write(x);

    x = 0;
    x.set_slc(0, ip.background.r.slc<3>(24));
    in.write(x);

    // GREEN COLOR
    x = 0;
    x.set_slc(0, ip.background.g.slc<12>(0));
    in.write(x);

    x = 0;
    x.set_slc(0, ip.background.g.slc<12>(12));
    in.write(x);

    x = 0;
    x.set_slc(0, ip.background.g.slc<3>(24));
    in.write(x); 

    // BLUE COLOR
    x = 0;
    x.set_slc(0, ip.background.b.slc<12>(0));
    in.write(x);

    x = 0;
    x.set_slc(0, ip.background.b.slc<12>(12));
    in.write(x);

    x = 0;
    x.set_slc(0, ip.background.b.slc<3>(24));
    in.write(x); 

    x = ip.image_height;
    in.write(x);

    x = ip.image_width;
    in.write(x);

    x = 0;
    x.set_slc(0, ip.center.x.slc<11>(0));
    in.write(x);

    x = 0;
    x.set_slc(0, ip.center.y.slc<11>(0));
    in.write(x);

    x = 0;
    x.set_slc(0, ip.center.z.slc<11>(0));
    in.write(x);

    // UPPER LEFT PIXEL 0,0 LOCATION /////////////////////////////////////
    x = 0;
    x.set_slc(0, ip.pixel00_loc.x.slc<11>(0));
    in.write(x);

    x = 0;
    x.set_slc(0, ip.pixel00_loc.x.slc<11>(11));
    in.write(x);

    x = 0;
    x.set_slc(0, ip.pixel00_loc.x.slc<11>(22));
    in.write(x); 

    x = 0;
    x.set_slc(0, ip.pixel00_loc.y.slc<11>(0));
    in.write(x);

    x = 0;
    x.set_slc(0, ip.pixel00_loc.y.slc<11>(11));
    in.write(x);

    x = 0;
    x.set_slc(0, ip.pixel00_loc.y.slc<11>(22));
    in.write(x); 

    x = 0;
    x.set_slc(0, ip.pixel00_loc.z.slc<11>(0));
    in.write(x);

    x = 0;
    x.set_slc(0, ip.pixel00_loc.z.slc<11>(11));
    in.write(x);

    x = 0;
    x.set_slc(0, ip.pixel00_loc.z.slc<11>(22));
    in.write(x); 

    // PIXEL DELTA U //////////////////////////////////////////////////////

    x = 0;
    x.set_slc(0, ip.pixel_delta_u.x.slc<11>(0));
    in.write(x);

    x = 0;
    x.set_slc(0, ip.pixel_delta_u.x.slc<11>(11));
    in.write(x);

    x = 0;
    x.set_slc(0, ip.pixel_delta_u.x.slc<3>(22));
    in.write(x); 

    x = 0;
    x.set_slc(0, ip.pixel_delta_u.y.slc<11>(0));
    in.write(x);

    x = 0;
    x.set_slc(0, ip.pixel_delta_u.y.slc<11>(11));
    in.write(x);

    x = 0;
    x.set_slc(0, ip.pixel_delta_u.y.slc<3>(22));
    in.write(x); 

    x = 0;
    x.set_slc(0, ip.pixel_delta_u.z.slc<11>(0));
    in.write(x);

    x = 0;
    x.set_slc(0, ip.pixel_delta_u.z.slc<11>(11));
    in.write(x);

    x = 0;
    x.set_slc(0, ip.pixel_delta_u.z.slc<3>(22));
    in.write(x); 

    // PIXEL DELTA V //////////////////////////////////////////////////////

    x = 0;
    x.set_slc(0, ip.pixel_delta_v.x.slc<11>(0));
    in.write(x);

    x = 0;
    x.set_slc(0, ip.pixel_delta_v.x.slc<11>(11));
    in.write(x);

    x = 0;
    x.set_slc(0, ip.pixel_delta_v.x.slc<3>(22));
    in.write(x); 

    x = 0;
    x.set_slc(0, ip.pixel_delta_v.y.slc<11>(0));
    in.write(x);

    x = 0;
    x.set_slc(0, ip.pixel_delta_v.y.slc<11>(11));
    in.write(x);

    x = 0;
    x.set_slc(0, ip.pixel_delta_v.y.slc<3>(22));
    in.write(x); 

    x = 0;
    x.set_slc(0, ip.pixel_delta_v.z.slc<11>(0));
    in.write(x);

    x = 0;
    x.set_slc(0, ip.pixel_delta_v.z.slc<11>(11));
    in.write(x);

    x = 0;
    x.set_slc(0, ip.pixel_delta_v.z.slc<3>(22));
    in.write(x); 

    // STREAM IN QUADS
    for (int i = 0; i < quads.size(); i++) {
        x = 0;
        x.set_slc(0, quads[i].corner_pt.x.slc<12>(0));
        in.write(x);

        x = 0;
        x.set_slc(0, quads[i].corner_pt.y.slc<12>(0));
        in.write(x);

        x = 0;
        x.set_slc(0, quads[i].corner_pt.z.slc<12>(0));
        in.write(x);

        x = 0;
        x.set_slc(0, quads[i].u.x.slc<12>(0));
        in.write(x);

        x = 0;
        x.set_slc(0, quads[i].u.y.slc<12>(0));
        in.write(x);

        x = 0;
        x.set_slc(0, quads[i].u.z.slc<12>(0));
        in.write(x);

        x = 0;
        x.set_slc(0, quads[i].v.x.slc<12>(0));
        in.write(x);

        x = 0;
        x.set_slc(0, quads[i].v.y.slc<12>(0));
        in.write(x);

        x = 0;
        x.set_slc(0, quads[i].v.z.slc<12>(0));
        in.write(x);

        x = quads[i].mat_type;
        in.write(x);

        x = quads[i].is_invis;
        in.write(x);

        // NORMAL VECTOR
        x = 0;
        x.set_slc(0, quads[i].normal.x.slc<12>(0));
        in.write(x);

        x = 0;
        x.set_slc(0, quads[i].normal.x.slc<12>(12));
        in.write(x);

        x = 0;
        x.set_slc(0, quads[i].normal.x.slc<2>(24));
        in.write(x); 

        x = 0;
        x.set_slc(0, quads[i].normal.y.slc<12>(0));
        in.write(x);

        x = 0;
        x.set_slc(0, quads[i].normal.y.slc<12>(12));
        in.write(x);

        x = 0;
        x.set_slc(0, quads[i].normal.y.slc<2>(24));
        in.write(x); 

        x = 0;
        x.set_slc(0, quads[i].normal.z.slc<12>(0));
        in.write(x);

        x = 0;
        x.set_slc(0, quads[i].normal.z.slc<12>(12));
        in.write(x);

        x = 0;
        x.set_slc(0, quads[i].normal.z.slc<2>(24));
        in.write(x); 

        // W VECTOR
        x = 0;
        x.set_slc(0, quads[i].w.x.slc<12>(0));
        in.write(x);

        x = 0;
        x.set_slc(0, quads[i].w.x.slc<12>(12));
        in.write(x);

        x = 0;
        x.set_slc(0, quads[i].w.x.slc<1>(24));
        in.write(x); 

        x = 0;
        x.set_slc(0, quads[i].w.y.slc<12>(0));
        in.write(x);

        x = 0;
        x.set_slc(0, quads[i].w.y.slc<12>(12));
        in.write(x);

        x = 0;
        x.set_slc(0, quads[i].w.y.slc<1>(24));
        in.write(x); 

        x = 0;
        x.set_slc(0, quads[i].w.z.slc<12>(0));
        in.write(x);

        x = 0;
        x.set_slc(0, quads[i].w.z.slc<12>(12));
        in.write(x);

        x = 0;
        x.set_slc(0, quads[i].w.z.slc<1>(24));
        in.write(x); 

        // D PLANE
        x = 0;
        x.set_slc(0, quads[i].d_plane.slc<12>(0));
        in.write(x);

        x = 0;
        x.set_slc(0, quads[i].d_plane.slc<12>(12));
        in.write(x);

        x = 0;
        x.set_slc(0, quads[i].d_plane.slc<7>(24));
        in.write(x); 

        // QUAD COLOR
        x = 0;
        x.set_slc(0, quads[i].quad_color.r.slc<12>(0));
        in.write(x);

        x = 0;
        x.set_slc(0, quads[i].quad_color.r.slc<12>(12));
        in.write(x);

        x = 0;
        x.set_slc(0, quads[i].quad_color.r.slc<3>(24));
        in.write(x); 

        x = 0;
        x.set_slc(0, quads[i].quad_color.g.slc<12>(0));
        in.write(x);

        x = 0;
        x.set_slc(0, quads[i].quad_color.g.slc<12>(12));
        in.write(x);

        x = 0;
        x.set_slc(0, quads[i].quad_color.g.slc<3>(24));
        in.write(x); 

        x = 0;
        x.set_slc(0, quads[i].quad_color.b.slc<12>(0));
        in.write(x);

        x = 0;
        x.set_slc(0, quads[i].quad_color.b.slc<12>(12));
        in.write(x);

        x = 0;
        x.set_slc(0, quads[i].quad_color.b.slc<3>(24));
        in.write(x); 

    }

    Pathtracer inst;

    // output channels for HLS model
    ac_channel<rgb_out> pixels_hls;

    printf("Running HLS design\n");

    // run the design
    inst.run(in, pixels_hls);

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

    for (int y = 0; y < image_height; y++) {
        for (int x = 0; x < image_width; x++) {
            rgb_out hout = pixels_hls.read();

            if ((abs(hout.r.to_int() - static_cast<int>(std::round(pixels_cpp.x()))) > 2) ||
                (abs(hout.g.to_int() - static_cast<int>(std::round(pixels_cpp.y()))) > 2) ||
                (abs(hout.b.to_int() - static_cast<int>(std::round(pixels_cpp.z()))) > 2) ||) {
                    mismatches++;
                }
        }
    }

    cout << "Test Completed with " << mismatches << " mismatches" << endl;

    for (int i = 0; i < image_height; i++) {
        delete[] pixels_cpp[i];
    }
    delete[] pixels_cpp;

    CCS_RETURN(0);
    
}