/*
The camera.h file introduces a camera class that models a simple pinhole camera in a ray-traced scene. 
It sets up the camera's position, orientation, the field of view, and the image dimensions. The render 
method is the core function that iterates over all pixels of the image, generating rays for each pixel
(with anti-aliasing through random sampling), and computing the color of the pixel by tracing these 
rays into the scene.

Key Components:
Camera Setup and Initialization: Prepares the camera based on its specifications, like position, 
orientation, and the image plane size.

Ray Generation: For each pixel, and for each sample within that pixel, a ray is generated and traced
into the scene.

Scene Rendering: The color of each pixel is determined by the cumulative color returned by the rays 
that hit objects in the scene. This process includes simulating light reflection and transmission 
based on materials.
*/

#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <thread>
#include <vector>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>
#include <fstream>  // Include for file operations
#define LONGEST_DISTANCE 2896
using std::sqrt;




 /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////           Fixed point          ////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

template<int num_f_bits>
class FixedPoint {
public:
    // Constructor from a double
    explicit FixedPoint(double value) {
        // Scaling the input value by the number of fractional bits and rounding to the nearest integer
        fixedValue = static_cast<int64_t>(std::round(value * (1LL << fractionalBits)));
    }

    // Default constructor
    FixedPoint() : fixedValue(0) {}

    // Addition operator
    FixedPoint operator+(const FixedPoint& other) const {
        return FixedPoint::fromRaw(fixedValue + other.fixedValue);
    }

    // Subtraction operator
    FixedPoint operator-(const FixedPoint& other) const {
        return FixedPoint::fromRaw(fixedValue - other.fixedValue);
    }

    // Division operator, using double for calculation
    FixedPoint operator/(const FixedPoint& other) const {
        if (other.fixedValue == 0) {
            std::cerr << "Division by zero error" << std::endl;
            return FixedPoint(0); // Handle division by zero if necessary
        }
        double result = this->toDouble() / other.toDouble(); // Convert both to double, divide
        return FixedPoint(result); // Convert back to fixed point
    }

    // Multiplication operator, using double for calculation
    FixedPoint operator*(const FixedPoint& other) const {
        double result = this->toDouble() * other.toDouble(); // Convert both to double, multiply
        return FixedPoint(result); // Convert back to fixed point
    }

    // Conversion to double
    double toDouble() const {
        return static_cast<double>(fixedValue) / (1LL << fractionalBits);
    }

    // Static factory method to create a FixedPoint from a raw fixed-point value (for internal use)
    static FixedPoint fromRaw(int64_t raw) {
        FixedPoint fp;
        fp.fixedValue = raw;
        return fp;
    }

private:
    int64_t fixedValue;  // The raw fixed-point value. Includes one sign bit implicitly.
    static const int fractionalBits = num_f_bits; // Number of fractional bits
};


 /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////           rtweekend          ////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Constants
const double infinity = 100000;  // A representation of infinity.
const double pi = 3.1415926535897932385;  // The value of Pi for use in conversions and calculations.


    class interval {
  public:
    double min, max;  // Represents the minimum and maximum values of the interval.

    // Default constructor initializes the interval to an "empty" state.
    interval() : min(+infinity), max(-infinity) {}

    // Constructor with specific min and max values.
    interval(double _min, double _max) : min(_min), max(_max) {}

    interval(const interval& a, const interval& b)
    : min(fmin(a.min, b.min)), max(fmax(a.max, b.max)) {}

    // Checks if a value is within the interval [min, max].
    bool contains(double x) const {
        return min <= x && x <= max;
    }

    // Checks if a value is strictly inside the interval (min, max).
    bool surrounds(double x) const {
        return min < x && x < max;
    }

    // Clamps a value to the interval, ensuring it lies within [min, max].
    double clamp(double x) const {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }

    double size() const {
        return max - min;
    }

    interval expand(double delta) const {
        auto padding = delta/2;
        return interval(min - padding, max + padding);
    }

    // Predefined intervals representing "empty" and "universe" (all-encompassing) cases.
    static const interval empty, universe;
};


interval operator+(const interval& ival, double displacement) {
    return interval(ival.min + displacement, ival.max + displacement);
}

interval operator+(double displacement, const interval& ival) {
    return ival + displacement;
}


// Definitions for the static members `empty` and `universe`.
const static interval empty(+infinity, -infinity);
const static interval universe(-infinity, +infinity);


    using std::shared_ptr;    // Allows for the use of smart pointers without specifying the namespace.
using std::make_shared;   // Simplifies the creation of smart pointers.
using std::sqrt;          // Provides access to the square root function.



// Utility Functions
inline double degrees_to_radians(double degrees) {
    // Converts degrees to radians for angle measurements.
    return degrees * pi / 180.0;
}


inline uint32_t xorshift32(uint32_t &state) {
    uint32_t x = state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    state = x;
    return x;
}

inline double random_double() {
    // // Returns a random real number in [0,1).

    static uint32_t state = 375821;  // Seed with a non-zero value
    uint32_t result = xorshift32(state);
    return static_cast<double>(result) / static_cast<double>(UINT32_MAX);

}

double uint32ToFixedPoint(uint32_t value) {
    // Maximum value for 32 fractional bits
    const double maxFractionalValue = 4294967296.0;  // 2^32

    // Convert the unsigned integer to a double as the fraction of the maximum possible value
    return value / maxFractionalValue;
}

inline double random_double1() {
    // // Returns a random real number in [0,1).

    static uint32_t state = 375821;  // Seed with a non-zero value
    uint32_t result = xorshift32(state);
    return uint32ToFixedPoint(result);

}

inline double random_double2() {
    // // Returns a random real number in [0,1).

    static uint32_t state = 39251088;  // Seed with a non-zero value
    uint32_t result = xorshift32(state);
    return uint32ToFixedPoint(result);

}


inline double random_double3() {
    // // Returns a random real number in [0,1).

    static uint32_t state = 375821;  // Seed with a non-zero value
    uint32_t result = xorshift32(state);
    return uint32ToFixedPoint(result);

}

inline double random_double4() {
    // // Returns a random real number in [0,1).

    static uint32_t state = 39251088;  // Seed with a non-zero value
    uint32_t result = xorshift32(state);
    return uint32ToFixedPoint(result);

}


 /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////           vec3          ////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////


// Defines a three-dimensional vector used for points, colors, etc.
class cpp_vec3 {
  public:
    double e[3];  // Stores the three components of the vector.

    // Constructors.
    cpp_vec3() : e{0,0,0} {}  // Default constructor initializes to zero.
    cpp_vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}  // Initializes with specified values.

    // Accessors for the vector components, useful for readability and when using the vector as a point or color.
    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    // Unary minus returns the negation of the vector, useful for reversing directions.
    cpp_vec3 operator-() const { return cpp_vec3(-e[0], -e[1], -e[2]); }
    // Subscript operators for accessing components by index.
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    // Adds the components of another cpp_vec3 to this one. Useful for vector addition.
    cpp_vec3& operator+=(const cpp_vec3 &v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    // Multiplies the components by a scalar. Useful for scaling vectors.
    cpp_vec3& operator*=(double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    // Divides the components by a scalar. Useful for normalizing vectors or scaling them down.
    cpp_vec3& operator/=(double t) {
        return *this *= 1/t;
    }

    // Computes the length (magnitude) of the vector. Essential for normalization and distance calculations.
    double length() const {
        return sqrt(length_squared());
    }

    // Computes the square of the length. Faster than length() as it avoids a square root. Useful for comparisons.
    double length_squared() const {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    // Determines if the vector is close to zero in all dimensions. Important for avoiding divide-by-zero errors.
    bool near_zero() const {
        const auto s = 1.49011612e-8;
        return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
    }

};

// Aliasing cpp_vec3 as point3 for semantic clarity: points represent positions in space.
using point3 = cpp_vec3;

// Vector Utility Functions
// Utility function to output a cpp_vec3 to an ostream, typically used for debugging.
inline std::ostream& operator<<(std::ostream &out, const cpp_vec3 &v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

// Adds two cpp_vec3 vectors and returns the result. Useful for position, direction, and color calculations.
inline cpp_vec3 operator+(const cpp_vec3 &u, const cpp_vec3 &v) {
    return cpp_vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

// Subtracts vector v from vector u and returns the result. Useful for vector calculations like finding a ray direction.
inline cpp_vec3 operator-(const cpp_vec3 &u, const cpp_vec3 &v) {
    return cpp_vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

// Multiplies two cpp_vec3 vectors component-wise and returns the result. This can represent color blending.
inline cpp_vec3 operator*(const cpp_vec3 &u, const cpp_vec3 &v) {
    return cpp_vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

// Multiplies a cpp_vec3 vector by a scalar and returns the result. Useful for scaling vectors.
inline cpp_vec3 operator*(double t, const cpp_vec3 &v) {
    return cpp_vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

// Equivalent to the previous function, demonstrating commutativity of scalar multiplication.
inline cpp_vec3 operator*(const cpp_vec3 &v, double t) {
    return t * v;
}

// Divides a cpp_vec3 vector by a scalar (performs the multiplication by the reciprocal of the scalar).
inline cpp_vec3 operator/(cpp_vec3 v, double t) {
    return (1/t) * v;
}

// Computes the dot product of two cpp_vec3 vectors. Fundamental in calculating angles between vectors and for many shading calculations.
inline double dot(const cpp_vec3 &u, const cpp_vec3 &v) {
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

// Computes the cross product of two cpp_vec3 vectors. Useful for finding a vector perpendicular to the plane defined by two vectors.
inline cpp_vec3 cross(const cpp_vec3 &u, const cpp_vec3 &v) {
    return cpp_vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

// Normalizes a cpp_vec3 vector, scaling it to a length of 1. This is often required when dealing with directions.
inline cpp_vec3 unit_vector(cpp_vec3 v) {
    return v / v.length();
}

// Utility function to generate a random point inside a unit sphere. Frequently used in generating diffuse materials.
inline cpp_vec3 random_in_unit_sphere() {
    // while (true) {
    //     auto p = cpp_vec3::random(-1,1);
    //     if (p.length_squared() < 1) return p;
    // }
    float xi1 = random_double3(); // Uniformly distributed from 0 to 1
    float xi2 = random_double4(); // Uniformly distributed from 0 to 1

    float theta = 2.0 * 3.14159 * xi1; // Uniform distribution from 0 to 2π
    float phi = std::acos(2.0 * xi2 - 1.0); // Correctly distribute from -1 to 1 and acos

    float xs = std::sin(phi) * std::cos(theta); // Note the switch of phi and theta here
    float ys = std::sin(phi) * std::sin(theta);
    float zs = std::cos(phi); 
    // std::cout << "X11 CPP " << xi1  << std::endl;
    // std::cout << "X12 CPP " << xi2  << std::endl;
    // std::cout << "COS PHI IN CPP " << 2.0 * xi2 - 1.0 << std::endl;
    // std::cout << "COS PHI IN CPP " << phi << std::endl;
    // std::cout << "COS PHI IN CPP " << std::cos(phi) << std::endl;


    return cpp_vec3(xs, ys, zs);
}

// Generates a random unit vector. Useful in simulating diffuse reflections.
inline cpp_vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

 /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////           color         ////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

// using color = cpp_vec3;  // Defines color as an alias for cpp_vec3, representing RGB colors.

// Converts linear RGB component to gamma-corrected component.
inline double linear_to_gamma(double linear_component) {
    return sqrt(linear_component);  // Gamma correction with gamma=2.0.
}

// Outputs a color to an ostream, applying gamma correction and averaging over samples per pixel.
void write_color(std::ostream &out, cpp_vec3 pixel_color, int samples_per_pixel) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();


    // std::cout << pixel_color * 255 << std::endl;
    // Average the color by the number of samples to reduce noise (Anti-aliasing).
    auto scale = 1.0 / samples_per_pixel;
    r *= scale;
    g *= scale;
    b *= scale;

    // Apply gamma correction (simplified form for gamma=2.0).
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // Convert to integer [0,255] and output.
    static const interval intensity(0.000, 0.999);  // Clamps color to prevent overflow.
    out << static_cast<int>(256 * intensity.clamp(r)) << ' '
        << static_cast<int>(256 * intensity.clamp(g)) << ' '
        << static_cast<int>(256 * intensity.clamp(b)) << '\n';
}

 /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////           ray         ////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool isWithinTolerance(double num1, double num2, double tolerancePercent) {
        // Calculate the tolerance threshold based on the second number
        double tolerance = std::fabs(num2 * (tolerancePercent / 100.0));

        // Check if the absolute difference between the numbers is within the tolerance
        return std::fabs(num1 - num2) <= tolerance;
    }

class c_ray {
  public:

    c_ray() {}  // Default constructor.

    // Constructor initializing a ray with an origin and direction.
    c_ray(const point3& origin, const cpp_vec3& direction) : orig(origin), dir(direction) {}

    // Getters for the ray's origin and direction.
    point3 origin() const { return orig; }
    cpp_vec3 direction() const { return dir; }

    // Calculates a point along the ray at parameter t.
    point3 at(double t) const {
        return orig + t*dir;
    }

    bool first_ray; 

    point3 orig;  // The ray's origin point.
    cpp_vec3 dir;     // The ray's direction vector.
};



     /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////           hit record         ////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

struct hit_record {
  point3 p;           // The point in 3D space where the ray hits the object.
  cpp_vec3 normal;        // The normal vector at the hit point, pointing outward.
  int mat;  // Pointer to the material of the object. Determines how the surface interacts with light.
  bool front_face;    // True if the ray hits the front face of the object, false if the back.
  bool is_secondary_ray = false;
  cpp_vec3 mat_color;
  // Determines the correct orientation for the normal vector. Ensures normal points against the ray.
  void set_face_normal(const c_ray& r, const cpp_vec3& outward_normal) {
      front_face = dot(r.direction(), outward_normal) < 0;
      normal = front_face ? outward_normal : -outward_normal;
  }
};



     /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////           lambertain         ////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////


class lambertian {
  public:
    lambertian(const cpp_vec3& a) : albedo(a){}

    // Implements the scattering function for a diffuse material.
    // Scatters rays in random directions with no reflection.
    bool scatter(const c_ray& r_in, const hit_record& rec, cpp_vec3& attenuation, c_ray& scattered) {
      auto scatter_direction = rec.normal + random_in_unit_sphere();

      // Catch degenerate scatter direction
      if (scatter_direction.near_zero())
          scatter_direction = rec.normal;

      //2^-23
      if (std::abs(scatter_direction.x()) < 0.00000011920929) {scatter_direction.e[0] = scatter_direction.x() < 0 ? -0.00000011920929 : 0.00000011920929;}
      if (std::abs(scatter_direction.y()) < 0.00000011920929) {scatter_direction.e[1] = scatter_direction.y() < 0 ? -0.00000011920929 : 0.00000011920929;}
      if (std::abs(scatter_direction.z()) < 0.00000011920929) {scatter_direction.e[2] = scatter_direction.z() < 0 ? -0.00000011920929 : 0.00000011920929;}
      scatter_direction.e[0] = FixedPoint<23>(scatter_direction.x()).toDouble();
      scatter_direction.e[1] = FixedPoint<23>(scatter_direction.y()).toDouble();
      scatter_direction.e[2] = FixedPoint<23>(scatter_direction.z()).toDouble();

      scattered = c_ray(rec.p, scatter_direction);
      attenuation = albedo;  // The color is affected by the material's albedo.
      return true;
    }

    cpp_vec3 albedo;  // Represents the color and intensity of the scattered light.
    int matnumber = 0;
};

         /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////           quad hit        ////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

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
    if (t < 0.0009765625 || t > closest_so_far){
        // std::cout << ((t)) << " NOT INSIDE" <<  std::endl;
        return false;
    }
    // Determine the hit point lies within the planar shape using its plane coordinates.
    // t (30 bits * dir (23) + orig (1)
    
    auto intersection = r.at(t);
    auto tmp_inttt = intersection;
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
    // std::cout << ((tmp_inttt)) << " MADE IT TO END" <<  std::endl;
    // std::cout << ((t)) << " MADE IT TO END" <<  std::endl;
    // std::cout << ((r.dir.x())) << " MADE IT TO END" <<  std::endl;
    // std::cout << ((r.orig.x())) << " MADE IT TO END" <<  std::endl <<  std::endl;
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





 /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////          Main Camera         ////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////


class camera {
  public:
    double aspect_ratio = 1.0;  // Defines the aspect ratio of the image (width / height).
    int    image_width  = 100;  // Width of the image in pixels.
    int    samples_per_pixel = 10;   // Number of samples per pixel for anti-aliasing.
    int    max_depth         = 10;   // Max depth for ray bouncing. Limits recursion in ray_color.
    cpp_vec3  background;               // Scene background color
    //mt stuff
    double vfov = 90;  // Vertical field of view in degrees.
    point3 lookfrom = point3(0,0,-1);  // The position of the camera in the scene.
    point3 lookat   = point3(0,0,0);   // The point towards which the camera is directed.
    cpp_vec3   vup      = cpp_vec3(0,1,0);     // "Up" direction in the scene, used for camera orientation.

    cpp_vec3 pixel_sample_square() const {
        // Generate a random point within the square surrounding a pixel for anti-aliasing.
        auto px = -0.5 + random_double1();  // Random offset in the x direction.
        auto py = -0.5 + random_double2();  // Random offset in the y direction.
        return (px * pixel_delta_u) + (py * pixel_delta_v);  // Adjust by pixel size.
    }

    c_ray get_ray(int i, int j) const {
        // Generate a ray originating from the camera to pass through the pixel (i,j).

        auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);  // Center of the pixel.
        auto pixel_sample = pixel_center + pixel_sample_square();  // Apply random sampling within the pixel for anti-aliasing.

        auto ray_origin = center;  // Ray starts at the camera's position.
        auto ray_direction = pixel_sample - ray_origin;  // Direction from camera to sampled point.

        return c_ray(ray_origin, ray_direction);  // Construct and return the ray.
    }

    // Overrides hit method to check all objects in the list for the closest hit.
    bool worldhit(const std::vector<shared_ptr<quad>>& objects, const c_ray& r, hit_record& rec) const {
        hit_record temp_rec;
        bool hit_anything = false;
        double closest_so_far = LONGEST_DISTANCE;  // Tracks the closest hit so far.

        // Iterate over all objects, updating `closest_so_far` with the nearest hit.
        for (const auto& object : objects) {
            quad quado_in(object->Q, object->u, object->v, object->mat, object->invis, object->obj_color);
            if (quad_hit(r, quado_in , closest_so_far, temp_rec)) {
                hit_anything = true;
                rec = temp_rec;  // Update the record to the nearest hit.
            }
        }
        return hit_anything;  // Returns true if any object was hit.x
    }

    c_ray get_scattered(hit_record& input_hr, c_ray& ray_in){
        lambertian scatterfunc(input_hr.mat_color);
        c_ray scattered; // The ray that results from scattering
        cpp_vec3 attenuation; // The attenuation of the ray after scattering
        scatterfunc.scatter(ray_in, input_hr, attenuation, scattered);
        scattered.first_ray = false;
        return scattered;
    }

    void material_scatter(c_ray& r, bool wasHit, hit_record& rec, 
                            cpp_vec3& accumulated_color, cpp_vec3& current_attenuation, c_ray& scattered_ray) {
        // cpp_vec3 accumulated_color(0, 0, 0); // Initialize accumulated color as black
        // cpp_vec3 current_attenuation(1, 1, 1); // Start with no attenuation
        c_ray current_ray = r; // The current ray starts as the initial ray

        // Check if the ray hits the background
        if (!wasHit) {
            accumulated_color += current_attenuation * background; // Apply background color
            current_attenuation = cpp_vec3(0, 0, 0);
            scattered_ray = current_ray;
            return;
        }

        // Emitted light from the material (if any)
        if (rec.mat == 1){
            cpp_vec3 emitted = rec.mat_color;
            accumulated_color += current_attenuation * emitted; // Add emitted light to accumulated color
            current_attenuation = cpp_vec3(0, 0, 0);
            scattered_ray = current_ray;
        }
        else {
            lambertian scatterfunc(rec.mat_color);
            c_ray scattered; // The ray that results from scattering
            cpp_vec3 attenuation; // The attenuation of the ray after scattering
            scatterfunc.scatter(current_ray, rec, attenuation, scattered);
            current_attenuation = current_attenuation * attenuation; // Update attenuation
            scattered_ray = scattered;
        }
        return;
    }

    cpp_vec3 ray_color(const c_ray& r, int max_depth, const std::vector<shared_ptr<quad>>& world) const {
        cpp_vec3 accumulated_color(0, 0, 0); // Initialize accumulated color as black
        cpp_vec3 current_attenuation(1, 1, 1); // Start with no attenuation
        c_ray current_ray = r; // The current ray starts as the initial ray

        for(int depth = 0; depth < max_depth; ++depth) {
            hit_record rec;

            current_ray.first_ray = depth == 0 ? true : false;
            // Check if the ray hits the background
            if (!worldhit(world, current_ray, rec)) {
                accumulated_color += current_attenuation * background; // Apply background color
                break;
            }

            hit_record rec_tmp = rec;
            // 2^-10
            if (std::abs(rec_tmp.p.x()) < 0.0009765625) {rec_tmp.p.e[0] = rec_tmp.p.x() < 0 ? -0.0009765625 : 0.0009765625;}
            if (std::abs(rec_tmp.p.y()) < 0.0009765625) {rec_tmp.p.e[1] = rec_tmp.p.y() < 0 ? -0.0009765625 : 0.0009765625;}
            if (std::abs(rec_tmp.p.z()) < 0.0009765625) {rec_tmp.p.e[2] = rec_tmp.p.z() < 0 ? -0.0009765625 : 0.0009765625;}

            rec_tmp.p.e[0] = FixedPoint<10>(rec_tmp.p.x()).toDouble();
            rec_tmp.p.e[1] = FixedPoint<10>(rec_tmp.p.y()).toDouble();
            rec_tmp.p.e[2] = FixedPoint<10>(rec_tmp.p.z()).toDouble();

            // After the first iteration, any ray is considered a secondary ray
            rec_tmp.is_secondary_ray = depth > 0;

            // Emitted light from the material (if any)
            if (rec.mat == 1){
                cpp_vec3 emitted = rec_tmp.mat_color;
                accumulated_color += current_attenuation * emitted; // Add emitted light to accumulated color
                break;
            }
            else {
                lambertian scatterfunc(rec.mat_color);
                c_ray scattered; // The ray that results from scattering
                cpp_vec3 attenuation; // The attenuation of the ray after scattering
                scatterfunc.scatter(current_ray, rec_tmp, attenuation, scattered);
                current_attenuation = current_attenuation * attenuation; // Update attenuation
                current_ray = scattered;
            }
        }

        return accumulated_color;
    }

    void render(const std::vector<shared_ptr<quad>>& world) {
        initialize();

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";  // PPM header.

        // Loop over each pixel in the image, from top to bottom.
        for (int j = 0; j < image_height; ++j) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            // Loop over each pixel in a row, from left to right.
            for (int i = 0; i < image_width; ++i) {
                cpp_vec3 pixel_color(0,0,0);
                // Sample the pixel color multiple times and average the results for anti-aliasing.
                for (int sample = 0; sample < samples_per_pixel; ++sample) {
                    c_ray r = get_ray(i, j);  // Generate a ray for this pixel and sample.
                    r = c_ray(r.origin(), cpp_vec3(FixedPoint<23>(r.direction().x()).toDouble(), FixedPoint<23>(r.direction().y()).toDouble(), FixedPoint<23>(r.direction().z()).toDouble()));
                    pixel_color += ray_color(r, max_depth, world);  // Accumulate color.
                }
                write_color(std::cout, pixel_color, samples_per_pixel);  // Output color to PPM.
            }
        }

        std::clog << "\rDone.                 \n";  // Completion message.
    }

    int    image_height;   // Height of the image, computed from width and aspect ratio.
    point3 center;         // The position of the camera (same as lookfrom).
    point3 pixel00_loc;    // Location in space of the top-left pixel.
    cpp_vec3   pixel_delta_u;  // Vector to move one pixel to the right on the image plane.
    cpp_vec3   pixel_delta_v;  // Vector to move one pixel down on the image plane.
    cpp_vec3   u, v, w;        // Orthogonal basis vectors for the camera orientation.

    void initialize() {
        // Calculate the height of the image based on its width and aspect ratio.
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        center = lookfrom;  // The camera's position in space.

        // Calculate viewport dimensions based on field of view and camera orientation.
        auto focal_length = (lookfrom - lookat).length();  // Distance to the focus point.
        auto theta = degrees_to_radians(vfov);  // Convert FOV to radians for calculation.
        auto h = tan(theta/2);  // Half the viewport height, in scene units.
        auto viewport_height = 2 * h * focal_length;  // Total viewport height.
        auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);  // Total viewport width.

        // Calculate the camera's basis vectors for defining orientation.
        w = unit_vector(lookfrom - lookat);  // Direction opposite to where the camera is looking.
        u = unit_vector(cross(vup, w));  // Perpendicular to w and vup, right direction.
        v = cross(w, u);  // Perpendicular to both w and u, up direction.

        // Calculate vectors to iterate over the viewport in pixel increments.
        cpp_vec3 viewport_u = viewport_width * u;  // Vector spanning the viewport width.
        cpp_vec3 viewport_v = viewport_height * -v;  // Vector spanning the viewport height, inverted.

        // Calculate pixel size in world units on the viewport.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the world space position of the top-left pixel (0,0).
        auto viewport_upper_left = center - (focal_length * w) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

};


void write_color(const std::string &filename, const cpp_vec3 &pixel_color, int samples_per_pixel) {
    std::ofstream out(filename, std::ios::app);  // Open file for appending

    // Handle possible file opening errors
    if (!out.is_open()) {
        std::cerr << "Failed to open " << filename << " for writing.\n";
        return;
    }

    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Average the color by the number of samples to reduce noise (Anti-aliasing).
    auto scale = 1.0 / samples_per_pixel;
    r *= scale;
    g *= scale;
    b *= scale;

    // Apply gamma correction (simplified form for gamma=2.0).
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // Convert to integer [0,255] and output.
    static const interval intensity(0.000, 0.999);
    out << static_cast<int>(256 * intensity.clamp(r)) << ' '
        << static_cast<int>(256 * intensity.clamp(g)) << ' '
        << static_cast<int>(256 * intensity.clamp(b)) << '\n';

    out.close();  // Close the file
}

#endif