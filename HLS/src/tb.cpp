#include "renderer.h"
using namespace std;

CCS_MAIN(int argc, char** argv) {
    
    Renderer inst; // DUT

    vec3<32, 8, true> in(-2.5, -2.5, -2.5); // input
    vec3<32, 8, true> out; // output

    inst.run(in, out);

    CCS_RETURN(0);
}