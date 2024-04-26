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
    sphere(point3 _center, double _radius, shared_ptr<material> _material, const bool is_invis)
      : center(_center), radius(FixedPoint(_radius)), mat(_material), invis(is_invis)
    {
    }

    // Override the hit function to provide an implementation specific to spheres.
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        if (r.first_ray && invis){
            return false;
        }

        vec3 oc = r.origin() - center;  // Vector from ray origin to sphere center.
        FixedPoint a = r.direction().length_squared();  // Squared length of the ray direction vector.
        FixedPoint half_b = dot(oc, r.direction());  // Dot product for simplified quadratic formula.
        FixedPoint c = oc.length_squared() - radius*radius;  // Sphere equation component.

        // Calculate the discriminant to determine if the ray intersects the sphere.
        FixedPoint discriminant = half_b*half_b - a*c;
        if (discriminant.toDouble() < 0) return false;  // No intersection if discriminant is negative.
        
        // Calculate the nearest root that lies within the acceptable interval.
        FixedPoint sqrtd = FixedPoint(sqrt(discriminant.toDouble()));
        FixedPoint root = (FixedPoint(0)-half_b - sqrtd) / a;
        // Check if the first root is within the interval.
        if (!ray_t.surrounds(root.toDouble())) {
            root = (FixedPoint(0)-half_b + sqrtd) / a;  // Try the second root.
            if (!ray_t.surrounds(root.toDouble()))
                return false;  // No valid intersection within the interval.
        }

                // std::clog << ((FixedPoint(0)-half_b + sqrtd)/a).toDouble() << "\n";

        // Fill the hit_record structure with details of the intersection.
        rec.t = root;  // Parameter t at the intersection.
        rec.p = r.at(rec.t.toDouble());  // Point of intersection.
        vec3 outward_normal = (rec.p - center) / radius.toDouble();  // Calculate the outward normal.
        rec.set_face_normal(r, outward_normal);  // Set the face normal, adjusting for ray direction.
        rec.mat = mat;  // Assign the sphere's material to the hit record.

        return true;  // Intersection found.
    }


  private:
    point3 center;  // Center of the sphere.
    FixedPoint radius;  // Radius of the sphere.
    shared_ptr<material> mat;  // Material of the sphere.
    bool invis;
};

#endif
