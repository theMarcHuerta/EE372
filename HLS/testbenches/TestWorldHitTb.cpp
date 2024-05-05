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

#include "TestWorldHit.cpp"
#include "../src/path_tracer_gold_model/camera.h"
#include "read_stimulus.cpp"

#define LONGEST_DISTANCE 2896


CCS_MAIN(int argc, char** argv) {

    // read in N input data sets
    std::vector<ray> HLS_rays;
    std::vector<ray> HLS_ray_out;
    std::vector<ray> HLS_bounced_rays;
    std::vector<rgb_in> HLS_color_out;
    std::vector<rgb_in> HLS_atten_out;
    std::vector<quad_hittable> HLS_quads;
    // output to compare with
    std::vector<HitRecord> HLS_records;

    std::vector<c_ray> cpp_rays;
    std::vector<c_ray> cpp_bounced_rays;
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
    uint64_t num_hit_bounced_cpp = 0;

    std::vector<bool> cpp_hit;
    std::vector<bool> cpp_bounce_hit;

    for (int i = 0; i < cpp_rays.size(); ++i) {
        hit_record tmp_rec;
        cpp_hit.push_back(cam.worldhit(world, cpp_rays[i], tmp_rec));
        if (std::abs(tmp_rec.p.x()) < 0.0009765625) {tmp_rec.p.e[0] = tmp_rec.p.x() < 0 ? -0.0009765625 : 0.0009765625;}
        if (std::abs(tmp_rec.p.y()) < 0.0009765625) {tmp_rec.p.e[1] = tmp_rec.p.y() < 0 ? -0.0009765625 : 0.0009765625;}
        if (std::abs(tmp_rec.p.z()) < 0.0009765625) {tmp_rec.p.e[2] = tmp_rec.p.z() < 0 ? -0.0009765625 : 0.0009765625;}

        tmp_rec.p.e[0] = FixedPoint<10>(tmp_rec.p.x()).toDouble();
        tmp_rec.p.e[1] = FixedPoint<10>(tmp_rec.p.y()).toDouble();
        tmp_rec.p.e[2] = FixedPoint<10>(tmp_rec.p.z()).toDouble();
        cpp_records.push_back(tmp_rec);
    }

    // for (int i = 0; i < cpp_records.size(); i++){
    //     c_ray bounced_ray = cam.get_scattered(cpp_records[i], cpp_rays[i]);
    //     cpp_bounced_rays.push_back(bounced_ray);
    // }
     
    // for (int i = 0; i < cpp_bounced_rays.size(); i++){
    //     double closest_so_far = LONGEST_DISTANCE;
    //     hit_record tmp_rec;
    //     for (int quad_num = 0; quad_num < cpp_quads.size(); quad_num++){
    //         bool wasHit = quad_hit(cpp_bounced_rays[i], cpp_quads[quad_num], closest_so_far, tmp_rec);
    //         cpp_bounce_hit.push_back(wasHit);
    //         if (wasHit){
    //             num_hit_bounced_cpp++;
    //             //  cout << "WAS HIT CPP : " << wasHit << endl; 
    //         }
    //     }
    // }

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

    ac_channel<ray> ray_in_chan;
    ac_channel<buffer_obj_count> params_in_chan;
    ac_channel<quad_hittable> quads;
    ac_channel<rgb_in> attenuation_chan_in;
    ac_channel<rgb_in> accumalated_color_chan_in;

    ac_channel<rgb_in> attenuation_chan_out;
    ac_channel<rgb_in> accumalated_color_out;
    ac_channel<HitRecord> hit_out;
    ac_channel<ray> ray_out;
    ac_channel<bool> isHit;

    buffer_obj_count params_in;

    params_in.num_quads = cpp_quads.size();
    params_in.background = {0.0, 0.0, 0.0};

    rgb_in shader1_atten = {1, 1, 1};
    rgb_in shader1_color = {0, 0, 0};


    printf("Running HLS C design\n");
    TestWorldHit inst; // DUT
    for (int i = 0; i < cpp_records.size(); ++i) {
        for (int quad_num = 0; quad_num < cpp_quads.size(); quad_num++){
            quads.write(HLS_quads[quad_num]);
        }
        ray ray_in = HLS_rays[i];

        ray_in_chan.write(ray_in);
        params_in_chan.write(params_in);
        attenuation_chan_in.write(shader1_atten);
        accumalated_color_chan_in.write(shader1_color);

        inst.run(ray_in_chan, params_in_chan, quads, attenuation_chan_in, accumalated_color_chan_in, 
                attenuation_chan_out, accumalated_color_out, hit_out, ray_out, isHit);

        HLS_atten_out.push_back(attenuation_chan_out.read());
        HLS_color_out.push_back(accumalated_color_out.read());
        HLS_records.push_back(hit_out.read());
        HLS_ray_out.push_back(ray_out.read());
        bool wasHit = isHit.read();
        HLS_hit.push_back(wasHit);
        if (wasHit){
            num_hit_hls++;
        }
     }

    // for (int i = 0; i < cpp_bounced_rays.size(); i++){
    //     c_ray r = cpp_bounced_rays[i];
    //     // if (wasHit){
    //     //      cout << "WAS HIT CPP : " << wasHit << endl; 
    //     // }
    //     ray tmp_ray;

    //     vec3<ac_fixed<21, 11, true>> orig;
    //     orig.x = r.orig.e[0];
    //     orig.y = r.orig.e[1];
    //     orig.z = r.orig.e[2];

    //     vec3<ac_fixed<34, 11, true>> dir;
    //     dir.x = r.dir.e[0];
    //     dir.y = r.dir.e[1];
    //     dir.z = r.dir.e[2];

    //     tmp_ray.camera_ray = false;
    //     tmp_ray.orig = orig;
    //     tmp_ray.dir = dir;

    //     HLS_bounced_rays.push_back(tmp_ray);
    // }


    // for (int i = 0; i < HLS_bounced_rays.size(); i++){
    //     ac_fixed<47, 17, true> closest_so_far = LONGEST_DISTANCE;
    //     HitRecord tmp_rec;
    //     for (int quad_num = 0; quad_num < cpp_quads.size(); quad_num++){
    //         bool wasHit;
    //         inst.run(HLS_bounced_rays[i], closest_so_far, HLS_quads[quad_num], tmp_rec, wasHit);
    //         if (wasHit){
    //             num_hit_bounced_hls++;
    //         }
    //         HLS_bounced_hit.push_back(wasHit);
    //     }
    // }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////           Final Results         ///////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    uint64_t num_hit_same = 0;
    uint64_t hit_loc_same = 0;

    uint64_t mismatches = 0;
    uint64_t tot_intersection_tests = cpp_hit.size();
    for (int i = 0; i < tot_intersection_tests; i++){
        HitRecord hls_rec = HLS_records[i];
        hit_record cpp_rec = cpp_records[i];

        double tmp_loc_x = hls_rec.hit_loc.x.to_double();
        double tmp_loc_y = hls_rec.hit_loc.y.to_double();
        double tmp_loc_z = hls_rec.hit_loc.z.to_double();

        bool loc_x = isWithinTolerance(cpp_rec.p.e[0],  tmp_loc_x, 10.0);
        bool loc_y = isWithinTolerance(cpp_rec.p.e[1],  tmp_loc_y, 10.0);
        bool loc_z = isWithinTolerance(cpp_rec.p.e[2],  tmp_loc_z, 10.0);

        bool loc_close = loc_x && loc_y && loc_z;

        if (loc_close){
            hit_loc_same++;
        }

        if (HLS_hit[i] == true && cpp_hit[i] == true){
            num_hit_same++;
        }
        if (HLS_hit[i] != cpp_hit[i]){
            mismatches++;
        }
    }



    // uint64_t num_hit_same_bounced = 0;

    // uint64_t mismatches_bounced = 0;
    // uint64_t tot_intersection_tests_bounced = cpp_bounce_hit.size();
    // for (int i = 0; i < tot_intersection_tests_bounced; i++){
    //     if (HLS_bounced_hit[i] == true && cpp_bounce_hit[i] == true){
    //         num_hit_same_bounced++;
    //     }
    //     if (HLS_bounced_hit[i] != cpp_bounce_hit[i]){
    //         mismatches_bounced++;
    //     }
    // }

    cout << "Test Completed with " << mismatches << " mismatches" << endl;
    cout << "Number of Tests " << HLS_hit.size() << endl;
    cout << "Number of Rays " << cpp_rays.size() << endl;
    cout << "Number of Hits CPP " << num_hit_cpp << endl;    
    cout << "Number of Hits HLS " << num_hit_hls << endl;
    cout << "Number of Hits Shared " << num_hit_same << endl;
    // cout << endl;
    // cout << endl;
    // cout << "Bounced Test Completed with " << mismatches_bounced << " mismatches" << endl;
    // cout << "Bounced Number of Tests " << HLS_bounced_hit.size() << endl;
    // cout << "Bounced Number of Rays " << cpp_bounced_rays.size() << endl;
    // cout << "Bounced Number of Hits CPP " << num_hit_bounced_cpp << endl;    
    // cout << "Bounced Number of Hits HLS " << num_hit_bounced_hls << endl;
    // cout << "Bounced Number of Hits Shared " << num_hit_same_bounced << endl;
    CCS_RETURN(0);
    
}