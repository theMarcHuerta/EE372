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

#include "TestShaderCores.cpp"
#include "../src/path_tracer_gold_model/camera.h"
#include "read_stimulus.cpp"

#define LONGEST_DISTANCE 2896


CCS_MAIN(int argc, char** argv) {

    // read in N input data sets
    std::vector<ray> HLS_rays;
    std::vector<ray> HLS_bounced_rays;
    std::vector<rgb_in> HLS_color_out;
    std::vector<rgb_in> HLS_atten_out;
    std::vector<quad_hittable> HLS_quads;
    // output to compare with
    std::vector<HitRecord> HLS_records;

    std::vector<c_ray> cpp_rays;
    std::vector<c_ray> cpp_bounced_rays;
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

    int image_height = 30;
    int image_width = 30;

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = image_height;
    cam.samples_per_pixel = 32;
    cam.max_depth         = 8;
    cam.background        = cpp_vec3(0,0,0);

    cam.vfov     = 40;
    cam.lookfrom = point3(278, 278, -800);
    cam.lookat   = point3(278, 278, 0);
    cam.vup      = cpp_vec3(0,1,0);
    cam.initialize();

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////           Copy C++ structs to HLS structs         ////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////


     for (int j = 0; j < image_height; ++j) {
        for (int i = 0; i < image_width; ++i) {
            for (int sample = 0; sample < cam.samples_per_pixel; ++sample) {
                c_ray r = cam.get_ray(i, j);  // Generate a ray for this pixel and sample.
                r = c_ray(r.origin(), cpp_vec3(FixedPoint<23>(r.direction().x()).toDouble(), FixedPoint<23>(r.direction().y()).toDouble(), FixedPoint<23>(r.direction().z()).toDouble()));
                r.first_ray = true;
                // if (wasHit){
                //      cout << "WAS HIT CPP : " << wasHit << endl; 
                // }
                ray tmp_ray;

                vec3<ac_fixed<21, 11, true>> orig;
                orig.x = r.orig.e[0];
                orig.y = r.orig.e[1];
                orig.z = r.orig.e[2];

                vec3<ac_fixed<34, 11, true>> dir;
                dir.x = r.dir.e[0];
                dir.y = r.dir.e[1];
                dir.z = r.dir.e[2];

                tmp_ray.camera_ray = true;
                tmp_ray.orig = orig;
                tmp_ray.dir = dir;

                cpp_rays.push_back(r);
                HLS_rays.push_back(tmp_ray);
            }
        }
     }


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
    uint64_t num_hit_cpp = 0;
    uint64_t num_hit_bounced_cpp = 0;

     for (int j = 0; j < image_height; ++j) {
        for (int i = 0; i < image_width; ++i) {
            cpp_vec3 pixel_color(0,0,0);
            for (int sample = 0; sample < cam.samples_per_pixel; ++sample) {
                pixel_color += cam.ray_color(cpp_rays[j*(image_width*cam.samples_per_pixel) + i * cam.samples_per_pixel + sample], 8, world);
            }
            cpp_color_out.push_back(pixel_color);
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
    uint64_t num_hit_hls = 0;
    uint64_t num_hit_bounced_hls = 0;
    
    std::vector<bool> HLS_hit;
    std::vector<bool> HLS_bounced_hit;

    buffer_obj_count params_in_obj;

    params_in_obj.num_quads = HLS_quads.size();
    params_in_obj.background = {0.0, 0.0, 0.0};
    params_in_obj.lastsamp = false;
    params_in_obj.firstsamp = true;

    printf("Running HLS C design\n");

    TestShaderCores core;
    ac_channel<quad_hittable> quads_in; 
    ac_channel<ray> ray_in;
    ac_channel<buffer_obj_count> params_in;
    ac_channel<rgb_in> output_pxl_serial;

    for (int j = 0; j < image_height; ++j) {
        for (int i = 0; i < image_width; ++i) {
            for (int sample = 0; sample < cam.samples_per_pixel; ++sample) {
                for (int bounces = 0; bounces < 8; bounces++){
                    for (int quad_num = 0; quad_num < cpp_quads.size(); quad_num++){
                        quads_in.write(HLS_quads[quad_num]);
                    }
                }

                params_in_obj.firstsamp = false;
                if ((j == 0) && (i == 0) && (sample == 0)){
                    params_in_obj.firstsamp = true;
                }
                
                if ((j == image_height-1) && (i == image_width-1) && (sample == cam.samples_per_pixel-1)){
                    params_in_obj.lastsamp = true;
                    params_in.write(params_in_obj);
                    // for the extra bounce
                    for (int quad_num = 0; quad_num < cpp_quads.size(); quad_num++){
                        quads_in.write(HLS_quads[quad_num]);
                    }
                }

                for (int i = 0; i < 8; i++){
                    params_in.write(params_in_obj);
                }

                ray_in.write(HLS_rays[j*(image_width*cam.samples_per_pixel) + i * cam.samples_per_pixel + sample]);

            }
        }
    }

    core.run(quads_in, ray_in, params_in, output_pxl_serial);

    for (int j = 0; j < image_height; j++) {
        for (int i = 0; i < image_width; i++) {
            rgb_in accum_col = {0, 0, 0};
            for (int sample = 0; sample < cam.samples_per_pixel; sample++) {

                rgb_in tmp_out = output_pxl_serial.read();
                ac_fixed<28, 6, false> colx = tmp_out.r +  accum_col.r ;
                ac_fixed<28, 6, false> coly = tmp_out.g +  accum_col.g ;
                ac_fixed<28, 6, false> colz = tmp_out.b +  accum_col.b ;

                if (colx[27] == 1){
                    accum_col.r = 31;
                }
                else {
                    accum_col.r = colx;
                }

                if (coly[27] == 1){
                    accum_col.g = 31;
                }
                else {
                    accum_col.g = coly;
                }

                if (colz[27] == 1){
                    accum_col.b = 31;
                }
                else {
                    accum_col.b = colz;
                }
            }
            HLS_color_out.push_back(accum_col);
        }
    }


    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////           Final Results         ///////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("Getting results\n");
    uint64_t mismatches = 0;
    uint64_t tot_intersection_tests = HLS_color_out.size();
    uint64_t testss = 0;
    int captures = 0;
    int captures_cpp = 0;
    int captures_shared = 0;

    uint64_t avg_px_cpp = 0;
    uint64_t avg_px_hls = 0;
    std::string filenme = "hls_image_new_core.ppm";

    for (int i = 0; i < tot_intersection_tests; i++){
        testss++;

        rgb_in color_hls = HLS_color_out[i];
        cpp_vec3 color_cpp = cpp_color_out[i];

        ////////////////// COLOR OUT

        double tmp_col_x = color_hls.r.to_double();
        double tmp_col_y = color_hls.g.to_double();
        double tmp_col_z = color_hls.b.to_double();
        cpp_vec3 pxl_col = cpp_vec3(tmp_col_x, tmp_col_y, tmp_col_z);

        write_color(filenme, pxl_col, cam.samples_per_pixel);

        bool col_x = isWithinTolerance(color_cpp.e[0],  tmp_col_x, 10.0);
        bool col_y = isWithinTolerance(color_cpp.e[1],  tmp_col_y, 10.0);
        bool col_z = isWithinTolerance(color_cpp.e[2],  tmp_col_z, 10.0);

        ////////////////////////////////////////////////////////////////

        bool col_close = col_x && col_y && col_z;
        bool cpp_cap = (color_cpp.e[0] > 0) || (color_cpp.e[1] > 0)  || (color_cpp.e[2] > 0) ;
        bool hls_cap = (tmp_col_x > 0) || (tmp_col_y > 0) || (tmp_col_z > 0);
        if (hls_cap){
            // cout << "Ray HLS color out: " << tmp_col_x << " " << tmp_col_y << " " << tmp_col_z << endl;
            captures++;
            avg_px_hls += i;
        }
        if (cpp_cap){
            // cout << "Ray HLS color out: " << tmp_col_x << " " << tmp_col_y << " " << tmp_col_z << endl;
            captures_cpp++;
            avg_px_cpp += i;
        }
        if (cpp_cap && hls_cap){
            captures_shared++;
        }
        if (!(col_close)){
            mismatches++;
            // cout << "Ray HLS color out: " << tmp_col_x << " " << tmp_col_y << " " << tmp_col_z << endl;
            // cout << "Ray CPP color out: " << color_cpp.e[0] << " " << color_cpp.e[1] << " " << color_cpp.e[2] << endl;
            // if (mismatches == 10) break;
        }
        // if (i == 1300) break;
    }

    cout << "Test Completed with " << mismatches << " mismatches" << endl;
    cout << "Number of Tests " << testss << endl;
    cout << "HLS with color " << captures << endl;
    cout << "cpp with color " << captures_cpp << endl;
    cout << "shared color " << captures_shared << endl;
    cout << "Avg color position HLS: " << avg_px_hls/captures << endl;
    cout << "Avg color position cpp: " << avg_px_cpp/captures_cpp << endl;
    cout << endl;
    cout << endl;
    CCS_RETURN(0);
    
}