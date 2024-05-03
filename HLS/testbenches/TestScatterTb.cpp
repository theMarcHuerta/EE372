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

#include "TestScatter.cpp"
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

    for (int j = 0; j < image_height; ++j) {
        for (int i = 0; i < image_width; ++i) {
            double closest_so_far = LONGEST_DISTANCE;
            hit_record tmp_rec;
            for (int quad_num = 0; quad_num < cpp_quads.size(); quad_num++){
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
            if (std::abs(tmp_rec.p.x()) < 0.0009765625) {tmp_rec.p.e[0] = tmp_rec.p.x() < 0 ? -0.0009765625 : 0.0009765625;}
            if (std::abs(tmp_rec.p.y()) < 0.0009765625) {tmp_rec.p.e[1] = tmp_rec.p.y() < 0 ? -0.0009765625 : 0.0009765625;}
            if (std::abs(tmp_rec.p.z()) < 0.0009765625) {tmp_rec.p.e[2] = tmp_rec.p.z() < 0 ? -0.0009765625 : 0.0009765625;}

            tmp_rec.p.e[0] = FixedPoint<10>(tmp_rec.p.x()).toDouble();
            tmp_rec.p.e[1] = FixedPoint<10>(tmp_rec.p.y()).toDouble();
            tmp_rec.p.e[2] = FixedPoint<10>(tmp_rec.p.z()).toDouble();
            cpp_records.push_back(tmp_rec);
        }
    }

    for (int i = 0; i < cpp_records.size(); i++){
        cpp_vec3 accumulated_color(0, 0, 0); // Initialize accumulated color as black
        cpp_vec3 current_attenuation(1, 1, 1); // Start with no attenuation
        c_ray scattered_ray_out;
        cam.material_scatter(cpp_rays[i], cpp_hit[i], cpp_records[i], accumulated_color, current_attenuation, scattered_ray_out);
        cpp_bounced_rays.push_back(scattered_ray_out);
        cpp_atten_out.push_back(current_attenuation);
        cpp_color_out.push_back(accumulated_color);
        cpp_bounced_rays.push_back(scattered_ray_out);
        // if (i == 10000) break;
    }
     
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

    // std::vector<rgb_in> cpp_fb_col_out;
    // std::vector<rgb_in> cpp_fb_atten_out;

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

    printf("Running HLS C design\n");

    for (int i = 0; i < cpp_records.size(); i++){
        hit_record tmp_Rec_cpp = cpp_records[i];
        HitRecord tmp_rec_hls;

        vec3<ac_fixed<21, 11, true>> hit_loc;
        hit_loc.x = tmp_Rec_cpp.p.e[0];
        hit_loc.y = tmp_Rec_cpp.p.e[1];
        hit_loc.z = tmp_Rec_cpp.p.e[2];

        vec3<ac_fixed<26, 2, true>> normal;
        normal.x = tmp_Rec_cpp.normal.e[0];
        normal.y = tmp_Rec_cpp.normal.e[1];
        normal.z = tmp_Rec_cpp.normal.e[2];

        bool front_face = tmp_Rec_cpp.front_face;

        uint_3 mat = tmp_Rec_cpp.mat;

        rgb_in color;
        color.r = tmp_Rec_cpp.mat_color.e[0];
        color.g = tmp_Rec_cpp.mat_color.e[1];
        color.b = tmp_Rec_cpp.mat_color.e[2];

        tmp_rec_hls.color = color;
        tmp_rec_hls.mat = mat;
        tmp_rec_hls.front_face = front_face;
        tmp_rec_hls.normal = normal;
        tmp_rec_hls.hit_loc = hit_loc;

        HLS_records.push_back(tmp_rec_hls);
        HLS_hit.push_back(cpp_hit[i]);
    }



    rgb_in shader1_atten = {1, 1, 1};
    rgb_in shader1_color = {0, 0, 0};

    TestScatter scatpack;
    ac_channel<ray> ray_in;
    ac_channel<HitRecord> hit_in;
    ac_channel<rgb_in> attenuation_chan_in;
    ac_channel<rgb_in> accumalated_color_chan_in;
    ac_channel<bool> isHit;
    ac_channel<rgb_in> attenuation_chan_out;
    ac_channel<rgb_in> accumalated_color_out;
    ac_channel<ray> ray_out;
    for (int i = 0; i < cpp_records.size(); i++){
        ray_in.write(HLS_rays[i]);
        hit_in.write(HLS_records[i]);
        attenuation_chan_in.write(shader1_atten);
        accumalated_color_out.write(shader1_color);
        isHit.write(HLS_hit[i]);

        scatpack.run(ray_in, hit_in, attenuation_chan_in, accumalated_color_out, isHit,
                        attenuation_chan_out, accumalated_color_out, ray_out);
        
        ray tmp_ray = ray_out.read();
        rgb_in tmp_atten = attenuation_chan_out.read();
        rgb_in tmp_col = accumalated_color_out.read();

        HLS_bounced_rays.push_back(tmp_ray);
        HLS_atten_out.push_back(tmp_atten);
        HLS_color_out.push_back(tmp_col);

    }


    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////           Final Results         ///////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    uint64_t mismatches = 0;
    uint64_t tot_intersection_tests = cpp_records.size();


    for (int i = 0; i < tot_intersection_tests; i++){
        c_ray tmp_scattered_cpp = cpp_bounced_rays[i];
        ray tmp_scattered_hls = HLS_bounced_rays[i];

        double tmp_loc_x = tmp_scattered_hls.orig.x.to_double();
        double tmp_loc_y = tmp_scattered_hls.orig.y.to_double();
        double tmp_loc_z = tmp_scattered_hls.orig.z.to_double();
        // if (i == 0){
        //      cout << "HLS ray: " << tmp_loc_x << " " << tmp_loc_y << " " << tmp_loc_z << endl;
        //      cout << "CPP ray: " << tmp_scattered_cpp.orig.e[0] << " " << tmp_scattered_cpp.orig.e[1] << " " << tmp_scattered_cpp.orig.e[2] << endl;
        // }
        bool loc_x = isWithinTolerance(tmp_scattered_cpp.orig.e[0],  tmp_loc_x, 10.0);
        bool loc_y = isWithinTolerance(tmp_scattered_cpp.orig.e[1],  tmp_loc_y, 10.0);
        bool loc_z = isWithinTolerance(tmp_scattered_cpp.orig.e[2],  tmp_loc_z, 10.0);

        double tmp_dir_x = tmp_scattered_hls.dir.x.to_double();
        double tmp_dir_y = tmp_scattered_hls.dir.y.to_double();
        double tmp_dir_z = tmp_scattered_hls.dir.z.to_double();
        // if (i == 0){
        //      cout << "HLS ray: " << tmp_dir_x << " " << tmp_dir_y << " " << tmp_dir_z << endl;
        //      cout << "CPP ray: " << tmp_scattered_cpp.dir.e[0] << " " << tmp_scattered_cpp.dir.e[1] << " " << tmp_scattered_cpp.dir.e[2] << endl;
        // }
        bool dir_x = isWithinTolerance(tmp_scattered_cpp.dir.e[0],  tmp_dir_x, 10.0);
        bool dir_y = isWithinTolerance(tmp_scattered_cpp.dir.e[1],  tmp_dir_y, 10.0);
        bool dir_z = isWithinTolerance(tmp_scattered_cpp.dir.e[2],  tmp_dir_z, 10.0);

        bool loc_close = loc_x && loc_y && loc_z;
        bool dir_close = dir_x && dir_y && dir_z;

        if (!(loc_close && dir_close)){
            mismatches++;
            // cout << "Ray mismatch " << i << endl;
            // cout << "Ray HLS orig: " << tmp_loc_x << " " << tmp_loc_y << " " << tmp_loc_z << endl;
            // cout << "Ray CPP orig: " << tmp_scattered_cpp.orig.e[0] << " " << tmp_scattered_cpp.orig.e[1] << " " << tmp_scattered_cpp.orig.e[2] << endl;
            // cout << "Ray HLS dir: " << dir_x << " " << dir_y << " " << dir_z << endl;
            // cout << "Ray CPP dir: " << tmp_scattered_cpp.dir.e[0] << " " << tmp_scattered_cpp.dir.e[1] << " " << tmp_scattered_cpp.dir.e[2] << endl << endl;
            // // break;
        }
        // if (i == 1300) break;
    }

    cout << "Test Completed with " << mismatches << " mismatches" << endl;
    cout << "Number of Tests " << HLS_bounced_rays.size() << endl;
    cout << endl;
    cout << endl;
    CCS_RETURN(0);
    
}