/*

*/

#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"  // Include the base class for hittable objects.
#include "vec3.h"      // Include the 3D vector class for mathematical operations.

// Defines a sphere class that inherits from hittable, making it capable of being intersected by rays.
class sphere : public hittable {
  public:
    // Constructor for creating a sphere with a center point, radius, and material.
    sphere(point3 _center, double _radius, shared_ptr<material> _material)
      : center(_center), radius(_radius), mat(_material) {}

    // Override the hit function to provide an implementation specific to spheres.
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        vec3 oc = r.origin() - center;  // Vector from ray origin to sphere center.
        auto a = r.direction().length_squared();  // Squared length of the ray direction vector.
        auto half_b = dot(oc, r.direction());  // Dot product for simplified quadratic formula.
        auto c = oc.length_squared() - radius*radius;  // Sphere equation component.

        // Calculate the discriminant to determine if the ray intersects the sphere.
        auto discriminant = half_b*half_b - a*c;
        if (discriminant < 0) return false;  // No intersection if discriminant is negative.
        
        // Calculate the nearest root that lies within the acceptable interval.
        auto sqrtd = sqrt(discriminant);
        auto root = (-half_b - sqrtd) / a;
        // Check if the first root is within the interval.
        if (!ray_t.surrounds(root)) {
            root = (-half_b + sqrtd) / a;  // Try the second root.
            if (!ray_t.surrounds(root))
                return false;  // No valid intersection within the interval.
        }

        // Fill the hit_record structure with details of the intersection.
        rec.t = root;  // Parameter t at the intersection.
        rec.p = r.at(rec.t);  // Point of intersection.
        vec3 outward_normal = (rec.p - center) / radius;  // Calculate the outward normal.
        rec.set_face_normal(r, outward_normal);  // Set the face normal, adjusting for ray direction.
        rec.mat = mat;  // Assign the sphere's material to the hit record.

        return true;  // Intersection found.
    }

  private:
    point3 center;  // Center of the sphere.
    double radius;  // Radius of the sphere.
    shared_ptr<material> mat;  // Material of the sphere.
};

#endif
