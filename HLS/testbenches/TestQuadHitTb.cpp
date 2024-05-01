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

#include "TestQuadHit.cpp"
#include "../src/path_tracer_gold_model/camera.h"
#include "read_stimulus.cpp"

#define LONGEST_DISTANCE 2896


CCS_MAIN(int argc, char** argv) {

    // read in N input data sets
    std::vector<ray> HLS_rays;
    std::vector<quad_hittable> HLS_quads;
    // output to compare with
    std::vector<HitRecord> HLS_records;

    std::vector<c_ray> cpp_rays;
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
    //////////////////////           Copy C++ structs to HLS structs         ////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////


     for (int j = 0; j < image_height; ++j) {
        for (int i = 0; i < image_width; ++i) {
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
    std::vector<bool> cpp_hit;

     for (int j = 0; j < image_height; ++j) {
        for (int i = 0; i < image_width; ++i) {
            double closest_so_far = LONGEST_DISTANCE;
            for (int quad_num = 0; quad_num < cpp_quads.size(); quad_num++){
                hit_record tmp_rec;
                bool wasHit = quad_hit(cpp_rays[i + (j * image_width)], cpp_quads[quad_num], closest_so_far, tmp_rec);
                cpp_hit.push_back(wasHit);
                if (wasHit){
                    num_hit_cpp++;
                    //  cout << "WAS HIT CPP : " << wasHit << endl; 
                }
                // if (i > 40 && i < 80 && j == 120){
                //     cout << "WAS HIT CPP : " << wasHit << endl; 
                // }
            }
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
    
    std::vector<bool> HLS_hit;

    printf("Running HLS C design\n");
    TestQuadHit inst; // DUT
    for (int j = 0; j < image_height; ++j) {
        for (int i = 0; i < image_width; ++i) {
            ac_fixed<47, 17, true> closest_so_far = LONGEST_DISTANCE;
            for (int quad_num = 0; quad_num < cpp_quads.size(); quad_num++){
                HitRecord tmp_rec;
                bool wasHit;
                inst.run(HLS_rays[i + (j * image_width)], closest_so_far, HLS_quads[quad_num], tmp_rec, wasHit);
                // if (i > 40 && i < 80 && j == 120){
                //     cout << "WAS HIT HLS : " << wasHit << endl; 
                // }
                if (wasHit){
                    num_hit_hls++;
                    //  cout << "WAS HIT CPP : " << wasHit << endl; 
                }
                HLS_hit.push_back(wasHit);
            }
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
    uint64_t num_hit_same = 0;

    uint64_t mismatches = 0;
    uint64_t tot_intersection_tests = cpp_hit.size();
    for (int i = 0; i < tot_intersection_tests; i++){
        if (HLS_hit[i] == true && cpp_hit[i] == true){
            num_hit_same++;
        }
        if (HLS_hit[i] != cpp_hit[i]){
            mismatches++;
        }
    }

    cout << "Test Completed with " << mismatches << " mismatches" << endl;
    cout << "Number of Tests " << HLS_hit.size() << endl;
    cout << "Number of Rays " << cpp_rays.size() << endl;
    cout << "Number of Hits CPP " << num_hit_cpp << endl;    
    cout << "Number of Hits HLS " << num_hit_hls << endl;
    cout << "Number of Hits Shared " << num_hit_same << endl;
    CCS_RETURN(0);
    
}