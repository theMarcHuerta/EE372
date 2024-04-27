/*
The hittable.h file defines the abstract base class hittable for objects that can 
be intersected by rays. The hit function is a pure virtual function, meaning any 
derived class must implement this function to describe how rays intersect with 
the object. The hit_record struct holds information about an intersection, such 
as the point of intersection, the normal at the intersection, and the material 
properties at the point.

Key Components:

hit_record Struct: Contains details about the ray-object intersection. This 
information is used for shading calculations.

hittable Class: An abstract class that serves as a base for any object that 
can be intersected by a ray. It defines a common interface for ray intersection 
tests.

Material Pointer: Each hit_record contains a shared pointer to a material. 
This allows the material properties of the object hit by the ray to influence the color calculation.
*/

#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"          // Includes the definition of a ray.
#include "rtweekend.h"    // Includes common utilities and constants.

class material;  // Forward declaration of the material class to avoid circular dependencies.

class hit_record {
  public:
    point3 p;           // The point in 3D space where the ray hits the object.
    vec3 normal;        // The normal vector at the hit point, pointing outward.
    shared_ptr<material> mat;  // Pointer to the material of the object. Determines how the surface interacts with light.
    double t;           // The parameter `t` for the ray equation P(t) = A + tb. Represents how far along the ray the hit occurs.
    // double u;
    // double v;
    bool front_face;    // True if the ray hits the front face of the object, false if the back.
    bool is_secondary_ray = false;

    // Determines the correct orientation for the normal vector. Ensures normal points against the ray.
    void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
  public:
    virtual ~hittable() = default;  // Virtual destructor for proper cleanup of derived classes.

    // Pure virtual function that must be implemented by derived classes. Checks if the ray hits the object.
    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;

};

#endif
