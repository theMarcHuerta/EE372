#include <iostream>

#include "Renderer.cpp"

using namespace std;

CCS_MAIN(int argc, char** argv) {

    Renderer inst; // DUT

    vec3<fxp_32> in(-2.5, -2.5, -2.5); // input
    vec3<fxp_32> out; // output

    inst.run(in, out);

    cout << "(" << out.x << ", " << out.y << ", " << out.z << ")" << endl;
    cout << "Done!  " << endl;

    CCS_RETURN(0);
}