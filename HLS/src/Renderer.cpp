#pragma once

#ifdef __SYNTHESIS__
    #define LABEL(x) x:
#else
    #define LABEL(x) {}
#endif

#include "Renderer.h"

#pragma hls_design top
class Renderer {
  public:
    Renderer() {}

    #pragma hls_design interface
    void CCS_BLOCK(run)(vec3<fxp_32>& a, vec3<fxp_32>& b, vec3<fxp_32>& cross, fxp_32& dot, vec3<fxp_32>& unit) {
        cross = a.cross(b);
        dot = a.dot(b);
        unit = cross.unit();
    }
};