#ifndef QUAD_H
#define QUAD_H

#include "rtweekend.h"
#include "hittable.h"
#include "fixedpoint.h"
// #include "material.h" 

#include <cmath>

struct quad {
    
    point3 Q;
    cpp_vec3 u, v;
    int mat;
    cpp_vec3 normal;
    double D;
    cpp_vec3 w;
    bool invis;
    cpp_vec3 obj_color;

    quad(const point3& _Q, const cpp_vec3& _u, const cpp_vec3& _v, int m, const bool is_invis, cpp_vec3 quad_color)
      : Q(_Q), u(_u), v(_v), mat(m), invis(is_invis), obj_color(quad_color)
    {
        auto n = cross(u, v);
        // n = cpp_vec3(FixedPoint<24>(n.x()).toDouble(), FixedPoint<24>(n.y()).toDouble(), FixedPoint<24>(n.z()).toDouble());
        normal = unit_vector(n);
        normal = cpp_vec3(FixedPoint<24>(normal.x()).toDouble(), FixedPoint<24>(normal.y()).toDouble(), FixedPoint<24>(normal.z()).toDouble());
        D = FixedPoint<20>(dot(normal, Q)).toDouble();        
        // D = dot(normal, Q);
        w = n / dot(n,n);
        w = cpp_vec3(FixedPoint<24>(w.x()).toDouble(), FixedPoint<24>(w.y()).toDouble(), FixedPoint<24>(w.z()).toDouble());

        Q = cpp_vec3(FixedPoint<1>(Q.x()).toDouble(), FixedPoint<1>(Q.y()).toDouble(), FixedPoint<1>(Q.z()).toDouble());
        u = cpp_vec3(FixedPoint<1>(u.x()).toDouble(), FixedPoint<1>(u.y()).toDouble(), FixedPoint<1>(u.z()).toDouble());
        v = cpp_vec3(FixedPoint<1>(v.x()).toDouble(), FixedPoint<1>(v.y()).toDouble(), FixedPoint<1>(v.z()).toDouble());

    }

};

bool is_interior(double a, double b) {
    if ((a < 0) || (1 < a) || (b < 0) || (1 < b))
        return false;

    return true;
}

bool quad_hit(const c_ray& r, const quad& quado, double& closest_so_far, hit_record& rec) {
    if (r.first_ray && quado.invis){
        return false;
    }
    // std::cout << 0 << "\n"; // not invis
    auto denom = FixedPoint<47>(dot(quado.normal, r.direction())).toDouble(); //24 bit * 23bit = 47
    if (std::abs(denom) < 1.1920929e-7) {denom = denom < 0 ? -1.1920929e-7 : 1.1920929e-7;}
    denom = FixedPoint<23>(denom).toDouble();

    auto tmp_denom = FixedPoint<20>(denom).toDouble(); // so far 24 bits made no difference, 20 no difference, can just truncate here

    // // 2 ^ -40
    // if (std::abs(denom) < 9.094947e-13) {denom = denom < 0 ? 9.094947e-13 : 9.094947e-13;}
    // denom = FixedPoint<40>(denom).toDouble();
    
    // No hit if the ray is parallel to the plane.
    if (fabs(tmp_denom) < 1e-6) {
        return false;
    }
    // Return false if the hit point parameter t is outside the ray interval.
    auto dot_normal_ray_origin = FixedPoint<34>(dot(quado.normal, r.origin())).toDouble(); //(24fbit * 10fbit = max 34fbit)
    // 2 ^ -20
    if (std::abs(dot_normal_ray_origin) < 0.000000953674316) {dot_normal_ray_origin = dot_normal_ray_origin < 0 ? -0.0009765625 : 0.0009765625;}
    dot_normal_ray_origin = FixedPoint<20>(dot(quado.normal, r.origin())).toDouble();
    // 17 max integer bits (including sign), 
    // max d is 11_20, dotnormal is 13_20 then divided by a 11_23 // max result is 13+11 + (23+20) = 43
    auto t = (quado.D - dot_normal_ray_origin) / denom; // 20 fbit number minus result of (24fbit * 10fbit (max is 34fbit) truncated to 20)
    // then we divid by a 23 bit number // 20 fbit divided by 23fbit
    // double tmp_int = 0;
    // double frac_part = std::modf(t, &tmp_int);
            // 2^-30
    if (std::abs(t) < 9.3132257e-10) {t = t < 0 ? -9.3132257e-10 : 9.3132257e-10;}

    t = FixedPoint<30>(t).toDouble();
    // t = tmp_int + frac_part;
    if (t < 0.001 || t > closest_so_far){
        return false;
    }
    // Determine the hit point lies within the planar shape using its plane coordinates.
    // t (30 bits * dir (23) + orig (1)
    auto intersection = r.at(t);
    intersection = cpp_vec3(FixedPoint<23>(intersection.x()).toDouble(), 
                        FixedPoint<23>(intersection.y()).toDouble(), FixedPoint<23>(intersection.z()).toDouble());
    //PLANAR IS 22_23
    cpp_vec3 planar_hitpt_vector = intersection - quado.Q;
    // cross is 22_23 * 11_1 and 11_1 so result is 33_24 
    // w is 24 frac bits, and 1 int bit
    // but checked and all we need is 15 integer bits for the alpha and beta result max
    // so should be 15_24 result
    // then the result of the corss adn dot is 15_48 but we trunc to 23 to do the comparison
    //
    auto tmp_cross_v = cross(planar_hitpt_vector, quado.v);
    tmp_cross_v = cpp_vec3(FixedPoint<23>(tmp_cross_v.x()).toDouble(), 
                        FixedPoint<23>(tmp_cross_v.y()).toDouble(), FixedPoint<23>(tmp_cross_v.z()).toDouble());
    auto alpha = dot(quado.w, tmp_cross_v);
    alpha = FixedPoint<23>(alpha).toDouble();
    // 
    //
    auto tmp_cross_u = cross(quado.u, planar_hitpt_vector);
    tmp_cross_u = cpp_vec3(FixedPoint<23>(tmp_cross_u.x()).toDouble(), 
                FixedPoint<23>(tmp_cross_u.y()).toDouble(), FixedPoint<23>(tmp_cross_u.z()).toDouble());
    auto beta = dot(quado.w, tmp_cross_u);
    beta = FixedPoint<23>(beta).toDouble();

    if (!is_interior(alpha, beta)){
        return false;
    }
    closest_so_far = t;
    rec.p = intersection;
    rec.mat = quado.mat;
    rec.set_face_normal(r, quado.normal);
    rec.mat_color = quado.obj_color;

    return true;
}

// Rotation function around the y-axis for vectors
cpp_vec3 rotate_y(const cpp_vec3& v, double sin_theta, double cos_theta) {
    return cpp_vec3(cos_theta * v.x() + sin_theta * v.z(), v.y(), -sin_theta * v.x() + cos_theta * v.z());
}

inline void box(const point3& a, const point3& b, int mat, cpp_vec3 mat_color, double angle_degrees, std::vector<shared_ptr<quad>>& world)
{
    auto min = point3(fmin(a.x(), b.x()), fmin(a.y(), b.y()), fmin(a.z(), b.z()));
    auto max = point3(fmax(a.x(), b.x()), fmax(a.y(), b.y()), fmax(a.z(), b.z()));

    // Center of the box (only x and z are used for calculating the rotation)
    point3 center = 0.5 * (min + max);
    cpp_vec3 center_horizontal(center.x(), 0, center.z());

    // Vectors defining the box dimensions
    cpp_vec3 dx = cpp_vec3(max.x() - min.x(), 0, 0);
    cpp_vec3 dy = cpp_vec3(0, max.y() - min.y(), 0);
    cpp_vec3 dz = cpp_vec3(0, 0, max.z() - min.z());

    // Calculate sine and cosine for the rotation
    double radians = angle_degrees * pi / 180.0;
    double cos_theta = cos(radians);
    double sin_theta = sin(radians);

    // Rotate vectors
    cpp_vec3 rotated_dx = rotate_y(dx, sin_theta, cos_theta);
    cpp_vec3 rotated_dz = rotate_y(dz, sin_theta, cos_theta);

    // Apply rotation around the center by adjusting corner positions
    // Start at the bottom front left corner and rotate around the horizontal center
    point3 front_bottom_left = center_horizontal + rotate_y(min - center_horizontal, sin_theta, cos_theta);
    point3 front_bottom_right = front_bottom_left + rotated_dx;

    // Construct quads with rotated vectors and adjusted positions
    world.push_back(make_shared<quad>(front_bottom_left, rotated_dx, dy, mat, false, mat_color)); // Front
    world.push_back(make_shared<quad>(front_bottom_left, rotated_dx, dy, mat, false, mat_color)); // Back
    world.push_back(make_shared<quad>(front_bottom_right, rotated_dz, dy, mat, false, mat_color)); // Right
    world.push_back(make_shared<quad>(front_bottom_left, rotated_dz, dy, mat, false, mat_color)); // Left
    world.push_back(make_shared<quad>(front_bottom_left + dy, rotated_dx, rotated_dz, mat, false, mat_color)); // Top

}

#endif