#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <ac_channel.h>

#include "TestQuadHit.cpp"
#include "read_stimulus.cpp"

using namespace std;


CCS_MAIN(int argc, char** argv) {

    TestQuadHit inst; // DUT

    // read in N input data sets
    std::vector<ray<sfp_11_22>> rays;
    std::vector<_quad_hittable<sfp_11_22>> quads;

    rays = read_rays<sfp_11_22>("../cpp_closer_to_hls/test_stimulus_and_results/bit_acc_c_result/camera_rays.txt");
    quads = read_hittables<sfp_11_22>("../cpp_closer_to_hls/test_stimulus_and_results/bit_acc_c_result/quads_in.txt");

    // output to compare with
    std::vector<HitRecord<sfp_11_22>> records;

    // golden output from Marc's cpp code
    std::vector<output> gold_output = read_recs("../cpp_closer_to_hls/test_stimulus_and_results/bit_acc_c_result/first_hit_rec.txt");

    if (rays.size() != gold_output.size()) {
        cout << "Different number of Rays and Hit Records" << endl;
        cout << "Num Rays: " << rays.size() << "    HitRecords: " << gold_output.size() << endl;
        CCS_RETURN(1);
    }

    // convert vectors to ac channels for giving to test module
    ac_channel<ray<sfp_11_22>> ray_channel;
    ac_channel<_quad_hittable<sfp_11_22>> quad_channel;
    ac_channel<HitRecord<sfp_11_22>> record_channel;

    // add read in vectors to their respective channels
    for (int i = 0; i < ray_channel.size(); i++) {
        ray_channel.write(rays[i]);
    }

    for (int j = 0; j < quad_channel.size(); j++) {
        quad_channel.write(quads[j]);
    }

    inst.run(ray_channel, quad_channel, record_channel);

    unsigned long long rec_idx;
    unsigned long long mismatches;
    while (record_channel.available(1)) {
        HitRecord<sfp_11_22> hr = record_channel.read();
        if (!(gold_output[rec_idx] == hr)) {
            mismatches++;
        }
        rec_idx++;
    }

    cout << "Test Completed with " << mismatches << " mismatches" << endl;
    CCS_RETURN(0);
    
}