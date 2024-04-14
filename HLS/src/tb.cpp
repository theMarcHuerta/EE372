#include <iostream>

#include "renderer.h"
#include "vec3.h"

using namespace std;

CCS_MAIN(int argc, char** argv) {
    
    Renderer<32, 8, true> inst; // DUT

    vec3<32, 8, true> in(-2.5, -2.5, -2.5); // input
    vec3<32, 8, true> out; // output

    inst.run(in, out);

    cout << "Done!\n" << endl;

    CCS_RETURN(0);
}