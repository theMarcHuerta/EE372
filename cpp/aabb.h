#ifndef AABB_H
#define AABB_H

#include "rtweekend.h"

// Defines an axis-aligned bounding box with intervals along the x, y, and z axes.
class aabb {
  public:
    interval x, y, z; // Each axis of the box is defined by an interval representing the min and max along that axis.

    // Default constructor. The intervals are empty by default, indicating an empty bounding box.
    aabb() {}

    // Constructs an AABB directly from intervals along each axis.
    aabb(const interval& ix, const interval& iy, const interval& iz)
      : x(ix), y(iy), z(iz) { }

    // Constructs an AABB from two points, treating them as diagonal corners of the box.
    // This constructor doesn't assume which point is min or max, computing them as needed.
    aabb(const point3& a, const point3& b) {
        x = interval(fmin(a[0],b[0]), fmax(a[0],b[0]));
        y = interval(fmin(a[1],b[1]), fmax(a[1],b[1]));
        z = interval(fmin(a[2],b[2]), fmax(a[2],b[2]));
    }

    aabb pad() {
        // Return an AABB that has no side narrower than some delta, padding if necessary.
        double delta = 0.0001;
        interval new_x = (x.size() >= delta) ? x : x.expand(delta);
        interval new_y = (y.size() >= delta) ? y : y.expand(delta);
        interval new_z = (z.size() >= delta) ? z : z.expand(delta);

        return aabb(new_x, new_y, new_z);
    }

    // Returns the interval corresponding to the specified axis.
    // 0 = x-axis, 1 = y-axis, 2 = z-axis
    const interval& axis(int n) const {
        if (n == 1) return y;
        if (n == 2) return z;
        return x;
    }

    // Tests if a ray hits this bounding box within a given interval (ray_t).
    bool hit(const ray& r, interval ray_t) const {
        for (int a = 0; a < 3; a++) {
            auto orig = r.origin()[a]; // Origin component of the ray.

            // Check for parallel rays and early out if the ray cannot possibly intersect the AABB.
            if (r.direction()[a] == 0) {
                if (orig < axis(a).min || orig > axis(a).max) {
                    return false; // Ray is parallel and outside the AABB bounds for this axis.
                }
                // If the ray is parallel but inside the bounds, it doesn't disqualify the ray; continue checking other axes.
                continue;
            }

            auto invD = 1 / r.direction()[a]; // Inverse of ray direction component.

            auto t0 = (axis(a).min - orig) * invD; // Calculating entry point along this axis.
            auto t1 = (axis(a).max - orig) * invD; // Calculating exit point along this axis.

            // If the ray direction component is negative, we need to swap entry and exit points.
            if (invD < 0) std::swap(t0, t1);

            // Update ray_t to the latest [entry, exit] interval considering all axes.
            if (t0 > ray_t.min) ray_t.min = t0;
            if (t1 < ray_t.max) ray_t.max = t1;

            // If the exit point comes before the entry point, the ray misses the box.
            if (ray_t.max <= ray_t.min) return false;
        }
        return true; // The ray hits the box if it passes all tests.
    }

    // Constructs an AABB that encompasses two other AABBs.
    // This is useful in building a BVH, where a node's bounding box encloses its children's boxes.
    aabb(const aabb& box0, const aabb& box1) {
        x = interval(box0.x, box1.x);
        y = interval(box0.y, box1.y);
        z = interval(box0.z, box1.z);
    }
};

aabb operator+(const aabb& bbox, const vec3& offset) {
    return aabb(bbox.x + offset.x(), bbox.y + offset.y(), bbox.z + offset.z());
}

aabb operator+(const vec3& offset, const aabb& bbox) {
    return bbox + offset;
}

#endif

    // bool hit(const ray& r, interval ray_t) const {
    //     for (int a = 0; a < 3; a++) {
    //         auto t0 = fmin((axis(a).min - r.origin()[a]) / r.direction()[a],
    //                        (axis(a).max - r.origin()[a]) / r.direction()[a]);
    //         auto t1 = fmax((axis(a).min - r.origin()[a]) / r.direction()[a],
    //                        (axis(a).max - r.origin()[a]) / r.direction()[a]);
    //         ray_t.min = fmax(t0, ray_t.min);
    //         ray_t.max = fmin(t1, ray_t.max);
    //         if (ray_t.max <= ray_t.min)
    //             return false;
    //     }
    //     return true;
    // }