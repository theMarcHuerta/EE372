#ifndef QUAD_H
#define QUAD_H

#include "rtweekend.h"
#include "hittable.h"
#include "hittable_list.h"
#include "fixedpoint.h"
// #include "material.h" 

#include <cmath>

class quad : public hittable {
  public:
    quad(const point3& _Q, const vec3& _u, const vec3& _v, shared_ptr<material> m, const bool is_invis)
      : Q(_Q), u(_u), v(_v), mat(m), invis(is_invis)
    {
        auto n = cross(u, v);
        // n = vec3(FixedPoint<24>(n.x()).toDouble(), FixedPoint<24>(n.y()).toDouble(), FixedPoint<24>(n.z()).toDouble());
        normal = unit_vector(n);
        normal = vec3(FixedPoint<24>(normal.x()).toDouble(), FixedPoint<24>(normal.y()).toDouble(), FixedPoint<24>(normal.z()).toDouble());
        D = FixedPoint<20>(dot(normal, Q)).toDouble();        
        // D = dot(normal, Q);
        w = n / dot(n,n);
        w = vec3(FixedPoint<24>(w.x()).toDouble(), FixedPoint<24>(w.y()).toDouble(), FixedPoint<24>(w.z()).toDouble());

        Q = vec3(FixedPoint<1>(Q.x()).toDouble(), FixedPoint<1>(Q.y()).toDouble(), FixedPoint<1>(Q.z()).toDouble());
        u = vec3(FixedPoint<1>(u.x()).toDouble(), FixedPoint<1>(u.y()).toDouble(), FixedPoint<1>(u.z()).toDouble());
        v = vec3(FixedPoint<1>(v.x()).toDouble(), FixedPoint<1>(v.y()).toDouble(), FixedPoint<1>(v.z()).toDouble());

        // std::cout << Q << "\n";
        // std::cout << u << "\n";
        // std::cout << v << "\n";
        // std::cout << mat->matnum() << "\n";
        // std::cout << invis << "\n";
        // std::cout << normal << "\n";
        // std::cout << w << "\n";
        // std::cout << D << "\n";
        // std::cout << mat->colorofmat() << "\n\n";

    }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        std::cout << Q << "\n";
        std::cout << u << "\n";
        std::cout << v << "\n";
        std::cout << mat->matnum() << "\n";
        std::cout << invis << "\n";
        std::cout << normal << "\n";
        std::cout << w << "\n";
        std::cout << D << "\n";
        std::cout << mat->colorofmat() << "\n";
        if (r.first_ray && invis){
            std::cout << 1 << "\n"; // invis
            std::cout << 0 << "\n"; // NOT parallel
            std::cout << 0 << "\n"; // in range
            std::cout << 0 << "\n"; // in interior

            std::cout << 0 << "\n"; // denom
            std::cout << 0 << "\n"; // t
            std::cout << 0 << " " << 0 << " " << 0 << "\n"; // intersec
            std::cout << 0 << " " << 0 << " " << 0 << "\n";// hitpt
            std::cout << 0 << "\n"; // alpha
            std::cout << 0 << "\n\n"; // beta
            return false;
        }
        std::cout << 0 << "\n"; // not invis
        auto denom = FixedPoint<47>(dot(normal, r.direction())).toDouble(); //24 bit * 23bit = 47
        if (std::abs(denom) < 1.1920929e-7) {denom = denom < 0 ? -1.1920929e-7 : 1.1920929e-7;}
        denom = FixedPoint<23>(denom).toDouble();

        auto tmp_denom = FixedPoint<20>(denom).toDouble(); // so far 24 bits made no difference, 20 no difference, can just truncate here

        // // 2 ^ -40
        // if (std::abs(denom) < 9.094947e-13) {denom = denom < 0 ? 9.094947e-13 : 9.094947e-13;}
        // denom = FixedPoint<40>(denom).toDouble();
        
        // No hit if the ray is parallel to the plane.
        if (fabs(tmp_denom) < 1e-6) {
            std::cout << 1 << "\n"; // parallel
            std::cout << 0 << "\n"; // in range
            std::cout << 0 << "\n"; // in interior

            std::cout << denom << "\n"; // denom
            std::cout << 0 << "\n"; // t
            std::cout << 0 << " " << 0 << " " << 0 << "\n"; // intersec
            std::cout << 0 << " " << 0 << " " << 0 << "\n";// hitpt
            std::cout << 0 << "\n"; // alpha
            std::cout << 0 << "\n\n"; // beta
            return false;
        }
        std::cout << 0 << "\n"; // NOT parallel

        // Return false if the hit point parameter t is outside the ray interval.
        auto dot_normal_ray_origin = FixedPoint<34>(dot(normal, r.origin())).toDouble(); //(24fbit * 10fbit = max 34fbit)
        // 2 ^ -20
        if (std::abs(dot_normal_ray_origin) < 0.000000953674316) {dot_normal_ray_origin = dot_normal_ray_origin < 0 ? -0.0009765625 : 0.0009765625;}
        dot_normal_ray_origin = FixedPoint<20>(dot(normal, r.origin())).toDouble();
        // 17 max integer bits (including sign), 
        // max d is 11_20, dotnormal is 13_20 then divided by a 11_23 // max result is 13+11 + (23+20) = 43
        auto t = (D - dot_normal_ray_origin) / denom; // 20 fbit number minus result of (24fbit * 10fbit (max is 34fbit) truncated to 20)
        // then we divid by a 23 bit number // 20 fbit divided by 23fbit
        // double tmp_int = 0;
        // double frac_part = std::modf(t, &tmp_int);
                // 2^-30
        if (std::abs(t) < 9.3132257e-10) {t = t < 0 ? -9.3132257e-10 : 9.3132257e-10;}

        t = FixedPoint<30>(t).toDouble();
        // t = tmp_int + frac_part;
        if (!ray_t.contains(t)){
            std::cout << 1 << "\n"; // not in range
            std::cout << 0 << "\n"; // in interior

            std::cout << denom << "\n"; // denom
            std::cout << t << "\n"; // denom
            std::cout << 0 << " " << 0 << " " << 0 << "\n"; // intersec
            std::cout << 0 << " " << 0 << " " << 0 << "\n";// hitpt
            std::cout << 0 << "\n"; // alpha
            std::cout << 0 << "\n\n"; // beta
            return false;
        }
        std::cout << 0 << "\n"; // in range
    
        // Determine the hit point lies within the planar shape using its plane coordinates.
        // t (30 bits * dir (23) + orig (1)
        auto intersection = r.at(t);
        intersection = vec3(FixedPoint<23>(intersection.x()).toDouble(), 
                            FixedPoint<23>(intersection.y()).toDouble(), FixedPoint<23>(intersection.z()).toDouble());
        //PLANAR IS 22_23
        vec3 planar_hitpt_vector = intersection - Q;
        // cross is 22_23 * 11_1 and 11_1 so result is 33_24 
        // w is 24 frac bits, and 1 int bit
        // but checked and all we need is 15 integer bits for the alpha and beta result max
        // so should be 15_24 result
        // then the result of the corss adn dot is 15_48 but we trunc to 23 to do the comparison
        //
        auto tmp_cross_v = cross(planar_hitpt_vector, v);
        tmp_cross_v = vec3(FixedPoint<23>(tmp_cross_v.x()).toDouble(), 
                            FixedPoint<23>(tmp_cross_v.y()).toDouble(), FixedPoint<23>(tmp_cross_v.z()).toDouble());
        auto alpha = dot(w, tmp_cross_v);
        alpha = FixedPoint<23>(alpha).toDouble();
        // 
        //
        auto tmp_cross_u = cross(u, planar_hitpt_vector);
        tmp_cross_u = vec3(FixedPoint<23>(tmp_cross_u.x()).toDouble(), 
                    FixedPoint<23>(tmp_cross_u.y()).toDouble(), FixedPoint<23>(tmp_cross_u.z()).toDouble());
        auto beta = dot(w, tmp_cross_u);
        beta = FixedPoint<23>(beta).toDouble();

        if (!is_interior(alpha, beta)){
            std::cout << 1 << "\n"; // not in interior
            std::cout << denom << "\n"; // denom
            std::cout << t << "\n"; // t
            std::cout << intersection << "\n"; // intersec
            std::cout << planar_hitpt_vector << "\n"; // hitpt
            std::cout << alpha << "\n"; // alpha
            std::cout << beta << "\n\n"; // beta
            return false;
        }
        std::cout << 0 << "\n"; // in interior

        std::cout << denom << "\n"; // denom
        std::cout << t << "\n"; // t
        std::cout << intersection << "\n"; // intersec
        std::cout << planar_hitpt_vector << "\n"; // hitpt
        std::cout << alpha << "\n"; // alpha
        std::cout << beta << "\n\n"; // beta
        // Ray hits the 2D shape; set the rest of the hit record and return true.
        rec.t = t;
        rec.p = intersection;
        rec.mat = mat;
        rec.set_face_normal(r, normal);

        return true;
    }

    virtual bool is_interior(double a, double b) const {
        // Given the hit point in plane coordinates, return false if it is outside the
        // primitive, otherwise set the hit record UV coordinates and return true.

        if ((a < 0) || (1 < a) || (b < 0) || (1 < b))
            return false;

        return true;
    }

  private:
    point3 Q;
    vec3 u, v;
    shared_ptr<material> mat;
    vec3 normal;
    double D;
    vec3 w;
    bool invis;
};


// Rotation function around the y-axis for vectors
vec3 rotate_y(const vec3& v, double sin_theta, double cos_theta) {
    return vec3(cos_theta * v.x() + sin_theta * v.z(), v.y(), -sin_theta * v.x() + cos_theta * v.z());
}

inline void box(const point3& a, const point3& b, shared_ptr<material> mat, double angle_degrees, hittable_list& world)
{
    auto sides = make_shared<hittable_list>();
    auto min = point3(fmin(a.x(), b.x()), fmin(a.y(), b.y()), fmin(a.z(), b.z()));
    auto max = point3(fmax(a.x(), b.x()), fmax(a.y(), b.y()), fmax(a.z(), b.z()));

    // Center of the box (only x and z are used for calculating the rotation)
    point3 center = 0.5 * (min + max);
    vec3 center_horizontal(center.x(), 0, center.z());

    // Vectors defining the box dimensions
    vec3 dx = vec3(max.x() - min.x(), 0, 0);
    vec3 dy = vec3(0, max.y() - min.y(), 0);
    vec3 dz = vec3(0, 0, max.z() - min.z());

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
    world.add(make_shared<quad>(front_bottom_left, rotated_dx, dy, mat, false)); // Front
    world.add(make_shared<quad>(front_bottom_left, rotated_dx, dy, mat, false)); // Back
    world.add(make_shared<quad>(front_bottom_right, rotated_dz, dy, mat, false)); // Right
    world.add(make_shared<quad>(front_bottom_left, rotated_dz, dy, mat, false)); // Left
    world.add(make_shared<quad>(front_bottom_left + dy, rotated_dx, rotated_dz, mat, false)); // Top

}
#endif