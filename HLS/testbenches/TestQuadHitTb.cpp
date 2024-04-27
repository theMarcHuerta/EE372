#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>

#include "TestQuadHit.cpp"
#include "read_stimulus.cpp"

using namespace std;


CCS_MAIN(int argc, char** argv) {

    TestQuadHit inst; // DUT

    // read in N input data sets
    std::vector<ray<sfp_11_22>> rays;
    std::vector<_quad_hittable<sfp_11_22>> quads;

    rays = read_rays<sfp_11_22>("../cpp_closer_to_hls/test_stimulus_and_results/10bit_rec_ray_1samp/camera_ray_1b_1s_240p.txt");
    quads = read_hittables<sfp_11_22>("../cpp_closer_to_hls/test_stimulus_and_results/10bit_rec_ray_1samp/quads_in_scene.txt");

    // output to compare with
    std::vector<HitRecord<sfp_11_22>> records;

    // golden output from Marc's cpp code
    std::vector<output> gold_output = read_recs("../cpp_closer_to_hls/test_stimulus_and_results/10bit_rec_ray_1samp/hit_rec_1b_1s_240p.txt");


    if (rays.size() != gold_output.size()) {
        cout << "Different number of Rays and Hit Records" << endl;
        cout << "Num Rays: " << rays.size() << "    HitRecords: " << gold_output.size() << endl;
        CCS_RETURN(1);
    }

    int count = 0;

    // for every ray
    for (ray<sfp_11_22> r : rays) {
        // initialize new outputs for every new ray
        sfp_11_22 closest_so_far = WS_MAX_X;
        HitRecord<sfp_11_22> rec = {{0, 0, 0}, {0, 0, 0}, 0, 0, 0, {0, 0, 0}};
        
        // for every quad
        for (_quad_hittable<sfp_11_22> q : quads) {
            int qcount = 0;

            sfp_11_22 x = closest_so_far;
            inst.run(r, closest_so_far, q, rec);
            if (closest_so_far != x && count == 47) cout << "Quad " << qcount << " hit!" << endl;
            
            // if (count == 0) {
            //     cout << "Ray Orig In " << r.orig.x << " " << r.orig.y << " " << r.orig.z << endl;
            //     cout << "Ray Dir In " << r.dir.x << " " << r.dir.y << " " << r.dir.z << endl;
            //     count++; 
            // }

            // if (qcount == 1) {
            //     cout << "Quad w: " << q.w.x << " " << q.w.y << " " << q.w.z << endl;
            // }
            qcount++;
        }
        records.push_back(rec);
        count++;
    }

    if (records.size() != gold_output.size()) {
        cout << "Different number of Expected and Actual Records" << endl;
        cout << "Num Actual: " << records.size() << "    Num Expected: " << gold_output.size() << endl;
        CCS_RETURN(1);
    }

    // compare outputs
    for (int i = 0; i < records.size(); i++) {
        if (!(gold_output[i] == records[i])) {
            cout << "Test Failed on Iteration " << i << "   :(   " << endl;
            cout << "Expected: " << gold_output[i].hit_loc.x << " " << gold_output[i].hit_loc.y << " " << gold_output[i].hit_loc.z << endl;
            cout << "Actual: " << records[i].hit_loc.x << " " << records[i].hit_loc.y << " " << records[i].hit_loc.z << endl;
            CCS_RETURN(1);
        }
    }

    
    cout << "Test Passed!  " << endl;
    CCS_RETURN(0);
    
}