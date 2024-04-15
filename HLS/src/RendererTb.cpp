#include <iostream>

#include "Renderer.cpp"

using namespace std;

CCS_MAIN(int argc, char** argv) {

    Renderer inst; // DUT

    vec3<fxp_32> a(0.25, 0.50, 0); // input
    vec3<fxp_32> b(0.50, 0.50, -0.25); // input

    vec3<fxp_32> cross; // output
    fxp_32 dot; // output
    vec3<fxp_32> unit; // output

    inst.run(a, b, cross, dot, unit);

    cout << dot << endl;
    cout << "(" << cross.x << ", " << cross.y << ", " << cross.z << ")" << endl;
    cout << "(" << unit.x << ", " << unit.y << ", " << unit.z << ")" << endl;
    cout << "Done!  " << endl;

    CCS_RETURN(0);
}