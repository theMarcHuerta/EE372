#pragma once

#include "vec3.h"

template<int W, int I, bool SIGNED>
class Renderer {
  public:
    Renderer() {}

    #pragma hls_design top
    void CCS_BLOCK(run)(vec3<W, I, SIGNED>& in, vec3<W, I, SIGNED>& out) {
        vec3<W, I, SIGNED> v(8.5, 8.5, 8.5);
        out = in.negate().add(v);
    }
};