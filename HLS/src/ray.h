#pragma once

#include <ac_math.h>
#include <ac_fixed.h>
#include <ac_int.h>

#include "vec3.h"

// Defines a three-dimensional vector used for points, colors, etc.
template<typename T>
class ray {
  public:
    vec3<T> orig;
    vec3<T> dir;

    ray() {}
    ray(vec3<T> origin, vec3<T> direction) : orig(origin), dir(direction) {}

    vec3<T> at(T& t) {
        vec3<T> mult_result = dir.mult(t);
        return orig.add(mult_result);
    }

}    