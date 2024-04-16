#include <iostream>

#include "TestRand.cpp"

using namespace std;

CCS_MAIN(int argc, char** argv) {

    TestRand inst; // DUT

    sfp_11_22 rand_nums[128];

    inst.run(rand_nums);

    cout << "\nRandom Numbers:\n" << endl;
    for (int i = 0; i < 128; i++) {
        cout << rand_nums[i] << endl;
    }
    cout << "Done!  " << endl;

    CCS_RETURN(0);
}