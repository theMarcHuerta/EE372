/*
The "material.h" file encapsulates the behavior of different materials in the scene 
by defining how they interact with light through reflection, refraction, or 
absorption. This interaction is crucial for achieving realistic rendering, as it 
simulates the physical properties of materials.

Key Components:

Abstract material Class: Serves as a base for all material types, declaring the 
scatter method which derived classes must implement. This method determines the 
behavior of rays when they strike a surface.

lambertian Class: Models diffuse materials which scatter rays uniformly in all 
directions. It's characterized by an albedo which describes its inherent color.
metal Class: Represents shiny, metallic surfaces that reflect rays in a single 
direction, with an optional fuzz parameter to simulate roughness.

dielectric Class: Simulates transparent materials like glass or water, taking 
into account the index of refraction to calculate reflections and refractions 
according to Snell's law and Fresnel equations.
*/

#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"
#include "color.h"
#include "hittable.h"
#include "fixedpoint.h"
// #include "quad.h"

class hit_record;  // Forward declaration for usage in material methods.

class material {
  public:
    virtual ~material() = default;  // Virtual destructor ensures derived class destructors are called.

    virtual color emitted() const {
        return color(0,0,0);
    }

    virtual color colorofmat() const {
        return color(0,0,0);
    }

    // Pure virtual function for scattering rays off material. Must be implemented by derived classes.
    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;

    virtual int matnum () const {
      return 0;
    } 
};

class lambertian : public material {
  public:
    lambertian(const color& a) : albedo(a){}

    // Implements the scattering function for a diffuse material.
    // Scatters rays in random directions with no reflection.
    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
    const override {
      auto scatter_direction = rec.normal + random_unit_vector();

      // Catch degenerate scatter direction
      if (scatter_direction.near_zero())
          scatter_direction = rec.normal;

      // vec3 tmp_p = rec.p;
      // if (std::abs(rec.p.x()) < 1e-16) {tmp_p.e[0] = 1e-7;}
      // if (std::abs(rec.p.y()) < 1e-16) {tmp_p.e[1] = 1e-7;}
      // if (std::abs(rec.p.y()) < 1e-16) {tmp_p.e[2] = 1e-7;}
      //2^-23
      if (std::abs(scatter_direction.x()) < 0.00000011920929) {scatter_direction.e[0] = scatter_direction.x() < 0 ? -0.00000011920929 : 0.00000011920929;}
      if (std::abs(scatter_direction.y()) < 0.00000011920929) {scatter_direction.e[1] = scatter_direction.y() < 0 ? -0.00000011920929 : 0.00000011920929;}
      if (std::abs(scatter_direction.z()) < 0.00000011920929) {scatter_direction.e[2] = scatter_direction.z() < 0 ? -0.00000011920929 : 0.00000011920929;}
      scatter_direction.e[0] = FixedPoint<23>(scatter_direction.x()).toDouble();
      scatter_direction.e[1] = FixedPoint<23>(scatter_direction.y()).toDouble();
      scatter_direction.e[2] = FixedPoint<23>(scatter_direction.z()).toDouble();

      scattered = ray(rec.p, scatter_direction);
      attenuation = albedo;  // The color is affected by the material's albedo.
      return true;
    }


    color colorofmat() const override {
      return albedo;
    }

    int matnum() const override {
      return 0;
    }

  private:
    // It's a measure of how much light that hits a surface is diffusely reflected, as opposed to being absorbed or transmitted through
    color albedo;  // Represents the color and intensity of the scattered light.
};

class metal : public material {
  public:
    metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1){}

    // Implements the scattering function for a metallic material.
    // Reflects rays with a possibility of fuzziness.
    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
    const override {
      vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
      scattered = ray(rec.p, reflected + fuzz*random_unit_vector());
      attenuation = albedo;  // The color is the same, but the direction changes due to reflection.
      return (dot(scattered.direction(), rec.normal) > 0);
    }

    vec3 colorofmat() const override {
      return albedo;
    }

    int matnum() const override {
      if (fuzz < 0.1875){
        return 1;
      }
      else if (fuzz < .375) {
        return 2;
      } 
      else if (fuzz < .625) {
        return 3;
      }
      else if (fuzz < .8125) {
        return 4;
      }
      else if (fuzz < 1) {
        return 5;
      }
      else  {
        return 6;
      }
    }


  private:
    color albedo;
    double fuzz;  // Determines the fuzziness of the reflection.
};

class dielectric : public material {
  public:
    dielectric(double index_of_refraction) : ir(index_of_refraction) {}

    // Implements the scattering function for dielectric (transparent) materials.
    // Handles both reflection and refraction based on the angle of incidence.
    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
    const override {
        attenuation = color(1.0, 1.0, 1.0);  // Dielectric materials do not attenuate the light.
        double refraction_ratio = rec.front_face ? (1.0/ir) : ir;

        vec3 unit_direction = unit_vector(r_in.direction());
        double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        vec3 direction;

        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()) {
            direction = reflect(unit_direction, rec.normal);  // Reflect if it cannot refract.
        } else {
            direction = refract(unit_direction, rec.normal, refraction_ratio);  // Refract otherwise.
        }

        scattered = ray(rec.p, direction);
        return true;
    }

    vec3 colorofmat() const override {
      return color(0,0,0);
    }

    int matnum() const override {
      return 0;
    }

  private:
    double ir; // Index of Refraction for the material.

    // Calculates the reflectance using Schlick's approximation.
    static double reflectance(double cosine, double ref_idx) {
        auto r0 = (1-ref_idx) / (1+ref_idx);
        r0 = r0*r0;
        return r0 + (1-r0)*pow((1 - cosine),5);
    }
};

class diffuse_light : public material {
  public:
    diffuse_light(color c) : emit(c) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
    const override {
        return false;
    }

    color emitted() const override {
        return emit;
    }

    vec3 colorofmat() const override {
      return color(0,0,0);
    }

    int matnum() const override {
      return 7;
    }


  private:
    color emit;
};

#endif
