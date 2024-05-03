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

    std::ofstream outfile("actual.txt");

    TestQuadHit inst; // DUT

    // read in N input data sets
    std::vector<ray> rays;
    std::vector<quad_hittable> quads;

    rays = read_rays("../cpp_closer_to_hls/test_stimulus_and_results/bit_acc_c_result/camera_rays.txt");
    cout << "Read in " << rays.size() << " rays" << endl;

    quads = read_hittables("../cpp_closer_to_hls/test_stimulus_and_results/bit_acc_c_result/quads_in.txt");
    cout << "Read in " << quads.size() << " quads" << endl;

    // output to compare with
    std::vector<HitRecord> records;

    // golden output from Marc's cpp code
    std::vector<output> gold_output = read_recs("../cpp_closer_to_hls/test_stimulus_and_results/bit_acc_c_result/first_hit_rec.txt");
    cout << "Read in " << gold_output.size() << " gold outputs" << endl;

    if (rays.size() != gold_output.size()) {
        cout << "Different number of Rays and Hit Records" << endl;
        cout << "Num Rays: " << rays.size() << "    HitRecords: " << gold_output.size() << endl;
        CCS_RETURN(1);
    }

    // convert vectors to ac channels for giving to test module
    ac_channel<ray> ray_channel;
    ac_channel<quad_hittable> quad_channel;
    ac_channel<HitRecord> record_channel;

    // write all test rays to channel
    for (int i = 0; i < rays.size(); i++) {
        ray_channel.write(rays[i]);

        // for every ray, write all quads that need to be checked
        for (int j = 0; j < quads.size(); j++) {
            quad_channel.write(quads[j]);
        }
    }

    inst.run(ray_channel, quad_channel, record_channel);

    unsigned long long rec_idx = 0;
    unsigned long long mismatches = 0;
    while (record_channel.available(1)) {
        HitRecord hr = record_channel.read();
        if (!(gold_output[rec_idx] == hr)) {
            mismatches++;
        }
        rec_idx++;

        outfile << hr.hit_loc.x << " " << hr.hit_loc.y << " " << hr.hit_loc.z << endl;
        outfile << hr.normal.x << " " << hr.normal.y << " " << hr.normal.z << endl;
        outfile << hr.front_face << endl;
        outfile << hr.t << endl;
        outfile << hr.mat << endl;
        outfile << hr.color.r << " " << hr.color.g << " " << hr.color.b << endl;
        outfile << "\n";

    }
    cout << rec_idx << endl;

    cout << "Test Completed with " << mismatches << " mismatches" << endl;
    CCS_RETURN(0);
    
}