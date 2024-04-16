#include <iostream>

#include "Renderer.cpp"

using namespace std;

CCS_MAIN(int argc, char** argv) {

    Renderer inst; // DUT

    vec3<fxp_32> a(0.25, 0.50, 0); // input
    vec3<fxp_32> b(0.50, 0.50, -0.25); // input

    vec3<fxp_32> cross; // output
    fxp_32 dot; // output
    vec3<fxp_32> reflect; // output

    fxp_32 rand_nums[128];

    inst.run(a, b, cross, dot, reflect, rand_nums);

    cout << dot << endl;
    cout << "(" << cross.x << ", " << cross.y << ", " << cross.z << ")" << endl;
    cout << "(" << reflect.x << ", " << reflect.y << ", " << reflect.z << ")" << endl;
    cout << "\nRandom Numbers:\n" << endl;
    for (int i = 0; i < 128; i++) {
        cout << rand_nums[i] << endl;
    }
    cout << "Done!  " << endl;

    CCS_RETURN(0);
}