/*
The "ray.h" file introduces the ray class, which is fundamental to ray tracing. 
Rays are defined by an origin point and a direction vector, and this class 
provides a method to compute a point along the ray given a parameter t
*/
#ifndef RAY_H
#define RAY_H

#include "vec3.h"  // Include the vec3 class for using 3D vectors and points.

class ray {
  public:
    ray() {}  // Default constructor.

    // Constructor initializing a ray with an origin and direction.
    ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}

    // Getters for the ray's origin and direction.
    point3 origin() const { return orig; }
    vec3 direction() const { return dir; }

    // Calculates a point along the ray at parameter t.
    point3 at(double t) const {
        return orig + t*dir;
    }

  private:
    point3 orig;  // The ray's origin point.
    vec3 dir;     // The ray's direction vector.
};

#endif
