#ifndef QUAD_H
#define QUAD_H

#include "rtweekend.h"
#include "hittable.h"
#include "hittable_list.h"

#include <cmath>

class quad : public hittable {
  public:
    quad(const point3& _Q, const vec3& _u, const vec3& _v, shared_ptr<material> m, const bool is_invis)
      : Q(_Q), u(_u), v(_v), mat(m), invis(is_invis)
    {
        auto n = cross(u, v);
        normal = unit_vector(n);
        D = dot(normal, Q);
        w = n / dot(n,n).toDouble();
        std::clog << w << " W \n";
        std::clog << D.toDouble() << "D \n";
        std::clog << normal << " NORM \n";
        std::clog << n << " N\n\n";
    }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        if (r.first_ray && invis){
            return false;
        }
        auto denom = dot(normal, r.direction());
        // std::clog << "\n\n";
        // std::clog << normal << " normal\n";
        // std::clog << "\n";
        // std::clog << r.direction()<< "rdir\n";
        // std::clog << "\n";
        // std::clog << denom.toDouble() << "DENOM\n";


        // No hit if the ray is parallel to the plane.
        if ((fabs(denom.toDouble()) < 1e-8) ){
            // std::clog << "\n IS PARALLEL\n";
            return false;
        }

        // Return false if the hit point parameter t is outside the ray interval.
        auto t = (D - dot(normal, r.origin())) / denom;

        // std::clog << dot(normal, r.origin()).toDouble() << " dot of t \n";
        // std::clog << (D - dot(normal, r.origin())).toDouble() << " D- dot of t \n";
        // std::clog << ((D - dot(normal, r.origin())) / denom).toDouble() << " D- dot of t divided by denom\n";
        // std::clog << t.toDouble() << "T \n";
        if (!ray_t.contains(t.toDouble())){
            // std::clog << "\n RAY T NOT CONTAIN \n";
            return false;
        }

        // Determine the hit point lies within the planar shape using its plane coordinates.
        auto intersection = r.at(t.toDouble());
        vec3 planar_hitpt_vector = intersection - Q;
        auto alpha = dot(w, cross(planar_hitpt_vector, v));
        auto beta = dot(w, cross(u, planar_hitpt_vector));

        // std::clog <<cross(planar_hitpt_vector, v)<< " cross a\n";
        // std::clog <<dot(w, cross(planar_hitpt_vector, v)).toDouble()<< " dot a\n";
        // std::clog <<intersection<< " intersection\n";
        // std::clog <<planar_hitpt_vector<< " planar_hitpt_vector\n";
        // std::clog <<alpha.toDouble()<< " alpha\n";
        // std::clog <<beta.toDouble()<< " beta\n";

        if (!is_interior(alpha, beta, rec)){
            // std::clog << "\n NOT IN INTERIOR\n";
            return false;
        }
        // Ray hits the 2D shape; set the rest of the hit record and return true.
        rec.t = t;
        rec.p = intersection;
        rec.mat = mat;
        rec.set_face_normal(r, normal);

        return true;
    }

    virtual bool is_interior(FixedPoint a, FixedPoint b, hit_record& rec) const {
        // Given the hit point in plane coordinates, return false if it is outside the
        // primitive, otherwise set the hit record UV coordinates and return true.

        if ((a.toDouble() < 0) || (1 < a.toDouble()) || (b.toDouble() < 0) || (1 < b.toDouble()))
            return false;

        rec.u = a;
        rec.v = b;
        return true;
    }

  private:
    point3 Q;
    vec3 u, v;
    shared_ptr<material> mat;
    vec3 normal;
    FixedPoint D;
    vec3 w;
    bool invis;
};


// Rotation function around the y-axis for vectors
vec3 rotate_y(const vec3& v, double sin_theta, double cos_theta) {
    return vec3(cos_theta * v.x().toDouble() + sin_theta * v.z().toDouble(), v.y().toDouble(), -sin_theta * v.x().toDouble() + cos_theta * v.z().toDouble());
}

inline shared_ptr<hittable_list> box(const point3& a, const point3& b, shared_ptr<material> mat, double angle_degrees)
{
    auto sides = make_shared<hittable_list>();
    auto min = point3(fmin(a.x().toDouble(), b.x().toDouble()), fmin(a.y().toDouble(), b.y().toDouble()), fmin(a.z().toDouble(), b.z().toDouble()));
    auto max = point3(fmax(a.x().toDouble(), b.x().toDouble()), fmax(a.y().toDouble(), b.y().toDouble()), fmax(a.z().toDouble(), b.z().toDouble()));

    // Center of the box (only x and z are used for calculating the rotation)
    point3 center = 0.5 * (min + max);
    vec3 center_horizontal(center.x().toDouble(), 0, center.z().toDouble());

    // Vectors defining the box dimensions
    vec3 dx = vec3(max.x().toDouble() - min.x().toDouble(), 0, 0);
    vec3 dy = vec3(0, max.y().toDouble() - min.y().toDouble(), 0);
    vec3 dz = vec3(0, 0, max.z().toDouble() - min.z().toDouble());

    // Calculate sine and cosine for the rotation
    double radians = angle_degrees * pi / 180.0;
    double cos_theta = cos(radians);
    double sin_theta = sin(radians);

    // Rotate vectors
    vec3 rotated_dx = rotate_y(dx, sin_theta, cos_theta);
    vec3 rotated_dz = rotate_y(dz, sin_theta, cos_theta);

    // Apply rotation around the center by adjusting corner positions
    // Start at the bottom front left corner and rotate around the horizontal center
    point3 front_bottom_left = center_horizontal + rotate_y(min - center_horizontal, sin_theta, cos_theta);
    point3 front_bottom_right = front_bottom_left + rotated_dx;

    // Construct quads with rotated vectors and adjusted positions
    sides->add(make_shared<quad>(front_bottom_left, rotated_dx, dy, mat, false)); // Front
    sides->add(make_shared<quad>(front_bottom_left, rotated_dx, dy, mat, false)); // Back
    sides->add(make_shared<quad>(front_bottom_right, rotated_dz, dy, mat, false)); // Right
    sides->add(make_shared<quad>(front_bottom_left, rotated_dz, dy, mat, false)); // Left
    sides->add(make_shared<quad>(front_bottom_left + dy, rotated_dx, rotated_dz, mat, false)); // Top

    return sides;
}
#endif