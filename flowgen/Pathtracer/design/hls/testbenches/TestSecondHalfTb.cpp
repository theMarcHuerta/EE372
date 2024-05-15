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

#include "TestSecondHalf.cpp"
#include "../src/path_tracer_gold_model/camera.h"
#include "read_stimulus.cpp"

#define LONGEST_DISTANCE 2896


CCS_MAIN(int argc, char** argv) {

    // read in N input data sets
    std::vector<rgb_in> HLS_color_out;
    std::vector<rgb_in> HLS_atten_out;
    std::vector<quad_hittable> HLS_quads;
    // output to compare with
    std::vector<HitRecord> HLS_records;

    std::vector<cpp_vec3> cpp_color_out;
    std::vector<cpp_vec3> cpp_atten_out;
    std::vector<quad> cpp_quads;
    // output to compare with
    std::vector<hit_record> cpp_records;

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

    int image_height = 150;
    int image_width = 150;

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = image_height;
    cam.samples_per_pixel = 0;
    cam.max_depth         = 8;
    cam.background        = cpp_vec3(0,0,0);

    cam.vfov     = 40;
    cam.lookfrom = point3(278, 278, -800);
    cam.lookat   = point3(278, 278, 0);
    cam.vup      = cpp_vec3(0,1,0);
    cam.initialize();

    int spp = (cam.samples_per_pixel == 0) ? 32 :
              (cam.samples_per_pixel == 1) ? 64 :
              (cam.samples_per_pixel == 2) ? 256 : 1024;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////           Copy C++ structs to HLS structs         ////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////


    for (const auto& object : world) {
        quad quado(object->Q, object->u, object->v, object->mat, object->invis, object->obj_color);
        quad_hittable tmp_quad;

        vec3<ac_fixed<12, 11, true>> corner_pt;
        corner_pt.x = quado.Q.e[0];
        corner_pt.y = quado.Q.e[1];
        corner_pt.z = quado.Q.e[2];

        vec3<ac_fixed<12, 11, true>> u; // defining u component 
        u.x = quado.u.e[0];
        u.y = quado.u.e[1];
        u.z = quado.u.e[2];

        vec3<ac_fixed<12, 11, true>> v; // defining v componenet
        v.x = quado.v.e[0];
        v.y = quado.v.e[1];
        v.z = quado.v.e[2];

        uint_3 mat_type = quado.mat; // allows for 4 possible materials, light, lambertian, metallic/specular, diaelectric??

        bool is_invis = quado.invis;

        vec3<ac_fixed<26, 2, true>> normal; // cross of u and v, 2_24
        normal.x = quado.normal.e[0];
        normal.y = quado.normal.e[1];
        normal.z = quado.normal.e[2];
        
        vec3<ac_fixed<25, 1, true>> w; // dot of u and v, 1_24
        w.x = quado.w.e[0];
        w.y = quado.w.e[1];
        w.z = quado.w.e[2];

        ac_fixed<31, 11, true> d_plane = quado.D; // 11_20

        rgb_in quad_color;
        quad_color.r = quado.obj_color.e[0];
        quad_color.g = quado.obj_color.e[1];
        quad_color.b = quado.obj_color.e[2];

        tmp_quad.corner_pt = corner_pt;
        tmp_quad.u = u;
        tmp_quad.v = v;
        tmp_quad.mat_type = mat_type;
        tmp_quad.is_invis = is_invis;
        tmp_quad.normal = normal;
        tmp_quad.w = w;
        tmp_quad.d_plane = d_plane;
        tmp_quad.quad_color = quad_color;

        cpp_quads.push_back(quado);
        HLS_quads.push_back(tmp_quad);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////           Run c++ model         ///////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    // create gold model pixel array
    cpp_vec3** pixels_cpp = new cpp_vec3*[image_height];
    for (int j = 0; j < image_height; ++j) {
        pixels_cpp[j] = new cpp_vec3[image_width];  // Allocate memory for each row.
    }
    cam.render_gold(world, pixels_cpp);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////           Run HLS model         ///////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    img_params params_in;

    params_in.num_quads = HLS_quads.size();
    params_in.samp_per_pxl = 0;
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

    printf("Running HLS C design\n");

    TestSecondHalf inst;
    ac_channel<quad_hittable> quads_in;
    ac_channel<quad_hittable> quads_two;
    ac_channel<img_params> render_params;
    ac_channel<img_params> accum_params;
    ac_channel<rgb_out> output_pxl_serial;
    uint16_t quad_max_one = (HLS_quads.size() >> 1);
    uint16_t quad_max_two = HLS_quads.size() - quad_max_one;

    // std::cout << quad_max_one << std::endl;
    // std::cout << quad_max_two << std::endl;

    render_params.write(params_in);
    accum_params.write(params_in);

    for (int j = 0; j < image_height; ++j) {
        for (int i = 0; i < image_width; ++i) {
            for (int sample = 0; sample < spp; ++sample) {
                for (int bounces = 0; bounces < 8; bounces++){
                    for (int quad_num = 0; quad_num < quad_max_one; quad_num++){
                        quads_in.write(HLS_quads[quad_num]);
                    }
                    for (int quad_num = 0; quad_num < quad_max_two; quad_num++){
                        quads_two.write(HLS_quads[quad_num+quad_max_one]);
                    }
                }
            }
        }
    }
    // for the 9th iteration on the last sample
    for (int quad_num = 0; quad_num < quad_max_one; quad_num++){
        quads_in.write(HLS_quads[quad_num]);
    }
    for (int quad_num = 0; quad_num < quad_max_two; quad_num++){
        quads_two.write(HLS_quads[quad_num+quad_max_one]);
    }

    inst.run(quads_in, quads_two, render_params, accum_params, output_pxl_serial);

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
    std::ofstream out("second_half.ppm");  // Open file for appending
    // Handle possible file opening errors
    if (!out.is_open()) {
        std::cerr << "Failed to open file for writing.\n";
        CCS_RETURN(1);
    }

    // set up ppm file
    out << "P3\n";
    out << image_width << " " << image_height << "\n";
    out << "255\n";

    for (int y = 0; y < image_height; y++) {
        for (int x = 0; x < image_width; x++) {
            rgb_out hout = output_pxl_serial.read();

            out << hout.r << ' '
                << hout.g << ' '
                << hout.b << ' ';

            // out << static_cast<int>(std::round(pixels_cpp[y][x].x())) << ' '
            //     << static_cast<int>(std::round(pixels_cpp[y][x].y())) << ' '
            //     << static_cast<int>(std::round(pixels_cpp[y][x].z())) << ' ';


            if ((abs(hout.r.to_int() - static_cast<int>(std::round(pixels_cpp[y][x].x()))) > 2) ||
                (abs(hout.g.to_int() - static_cast<int>(std::round(pixels_cpp[y][x].y()))) > 2) ||
                (abs(hout.b.to_int() - static_cast<int>(std::round(pixels_cpp[y][x].z()))) > 2)) {
                    mismatches++;
                }
        }
        out << "\n";
    }

    out.close();  // Close the file

    cout << "Test Completed with " << mismatches << " mismatches" << endl;

    for (int i = 0; i < image_height; i++) {
        delete[] pixels_cpp[i];
    }
    delete[] pixels_cpp;

    CCS_RETURN(0);


}