
#include "../lib/mathlib.h"
#include <cmath>
#include <algorithm>

bool BBox::hit(const Ray& ray, Vec2& times) const {

    // TODO (PathTracer):
    // Implement ray - bounding box intersection test
    // If the ray intersected the bounding box within the range given by
    // [times.x,times.y], update times with the new intersection times.

    // hit distances
    float tx0, tx1, ty0, ty1, tz0, tz1;

    // plane locations
    float x0 = min.x;
    float x1 = max.x;
    float y0 = min.y;
    float y1 = max.y;
    float z0 = min.z;
    float z1 = max.z;

    // if there is no direction component of the ray in a certain direction, it will never intersect with those planes
    bool has_x_comp = (ray.dir.x != 0);
    bool has_y_comp = (ray.dir.y != 0);
    bool has_z_comp = (ray.dir.z != 0);

    // compute hit values
    if (has_x_comp) {
        float a_x = 1.0f/ray.dir.x;
        float b_x = -ray.point.x / ray.dir.x;
        tx0 = a_x*x0 + b_x;
        tx1 = a_x*x1 + b_x;
        if (tx0 > tx1) std::swap(tx0, tx1);
    } else {
        tx0 = -INFINITY;
        tx1 = INFINITY;
        // if ray has no x component, it can't hit the box if not already within the BBox's x bounds
        if ((ray.point.x < x0) || (ray.point.x > x1)) return false;
    }

    if (has_y_comp) {
        float a_y = 1.0f/ray.dir.y;
        float b_y = -ray.point.y / ray.dir.y;
        ty0 = a_y*y0 + b_y;
        ty1 = a_y*y1 + b_y;
        if (ty0 > ty1) std::swap(ty0, ty1);
    } else {
        ty0 = -INFINITY;
        ty1 = INFINITY;
        // if ray has no y component, it can't hit the box if not already within the BBox's y bounds
        if ((ray.point.y < y0) || (ray.point.y > y1)) return false;
    }

    if (has_z_comp) {
        float a_z = 1.0f/ray.dir.z;
        float b_z = -ray.point.z / ray.dir.z;
        tz0 = a_z*z0 + b_z;
        tz1 = a_z*z1 + b_z;
        if (tz0 > tz1) std::swap(tz0, tz1);
    } else {
        tz0 = -INFINITY;
        tz1 = INFINITY;
        // if ray has no z component, it can't hit the box if not already within the BBox's z bounds
        if ((ray.point.z < z0) || (ray.point.z > z1)) return false;
    }

    // calculate intersection bounds of hit ranges
    float time_lower_bound = std::max(std::max(tx0, ty0), tz0);
    float time_upper_bound = std::min(std::min(tx1, ty1), tz1);

    // if ranges don't have a union, we miss the box
    if (time_lower_bound > time_upper_bound) return false;
    
    // if ranges do have a union, update time bounds and return a hit
    times.x = std::max(time_lower_bound, times.x);
    times.y = std::min(time_upper_bound, times.y);

    return true;
}
