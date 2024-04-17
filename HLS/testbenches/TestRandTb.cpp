#include <iostream>

#include "TestRand.cpp"

using namespace std;

CCS_MAIN(int argc, char** argv) {

    TestRand inst; // DUT

    sfp_11_22 rand_nums[32];
    vec3<sfp_11_22> a;
    vec3<sfp_11_22> b;

    inst.run(rand_nums, a, b);

    cout << "\nRandom Numbers:\n" << endl;
    for (int i = 0; i < 32; i++) {
        cout << rand_nums[i] << endl;
    }
    cout << "(" << a.x << ", " << a.y << ", " << a.z << ")" << endl;
    cout << "(" << b.x << ", " << b.y << ", " << b.z << ")" << endl;
    cout << "Done!  " << endl;

    CCS_RETURN(0);
}