#include <iostream>

#include "TestVec.cpp"

using namespace std;

CCS_MAIN(int argc, char** argv) {

    TestVec inst; // DUT

    vec3<sfp_11_22> a(0.25, 0.50, 0); // input
    vec3<sfp_11_22> b(0.50, 0.50, -0.25); // input
    ray<sfp_11_22> r(vec3<sfp_11_22>(1.5, -2.75, 5), vec3<sfp_11_22>(-0.75, 1, 0)); // input

    vec3<sfp_11_22> cross; // output
    sfp_11_22 dot; // output
    vec3<sfp_11_22> reflect; // output
    vec3<sfp_11_22> at;

    inst.run(a, b, r, cross, dot, reflect, at);

    cout << dot << endl;
    cout << "(" << cross.x << ", " << cross.y << ", " << cross.z << ")" << endl;
    cout << "(" << reflect.x << ", " << reflect.y << ", " << reflect.z << ")" << endl;
    cout << "(" << at.x << ", " << at.y << ", " << at.z << ")" << endl;
    cout << "Done!  " << endl;

    CCS_RETURN(0);
}