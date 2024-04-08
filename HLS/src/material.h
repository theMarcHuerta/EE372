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
#include "texture.h"

class hit_record;  // Forward declaration for usage in material methods.

class material {
  public:
    virtual ~material() = default;  // Virtual destructor ensures derived class destructors are called.

    virtual color emitted(double u, double v, const point3& p) const {
        return color(0,0,0);
    }

    // Pure virtual function for scattering rays off material. Must be implemented by derived classes.
    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

class lambertian : public material {
  public:
    lambertian(const color& a) : albedo(make_shared<solid_color>(a)), is_invis(false) {}
    lambertian(shared_ptr<texture> a) : albedo(a), is_invis(false) {}

    // Implements the scattering function for a diffuse material.
    // Scatters rays in random directions with no reflection.
    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
    const override {
        if (rec.is_secondary_ray || (!is_invis)) {
          auto scatter_direction = rec.normal + random_unit_vector();

          // Catch degenerate scatter direction
          if (scatter_direction.near_zero())
              scatter_direction = rec.normal;

          scattered = ray(rec.p, scatter_direction);
          attenuation = albedo->value(rec.u, rec.v, rec.p);  // The color is affected by the material's albedo.
          return true;
        }
        else {
          // For primary rays, simulate passing through by continuing the ray unchanged.
          // This means setting the scattered ray to continue in the same direction.
          scattered = ray(rec.p - 0.001*rec.normal, r_in.direction()); // Offset along normal
          attenuation = color(1.0, 1.0, 1.0); // No attenuation for primary rays.
          return true;
        }
    }

    void set_invis() { is_invis = true; }

  private:
    // It's a measure of how much light that hits a surface is diffusely reflected, as opposed to being absorbed or transmitted through
    shared_ptr<texture> albedo;  // Represents the color and intensity of the scattered light.
    bool is_invis;
};

class metal : public material {
  public:
    metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1), is_invis(false) {}

    // Implements the scattering function for a metallic material.
    // Reflects rays with a possibility of fuzziness.
    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
    const override {
        if (rec.is_secondary_ray || (!is_invis)) {
          vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
          scattered = ray(rec.p, reflected + fuzz*random_unit_vector());
          attenuation = albedo;  // The color is the same, but the direction changes due to reflection.
          return (dot(scattered.direction(), rec.normal) > 0);
        }
        else {
          // For primary rays, simulate passing through by continuing the ray unchanged.
          // This means setting the scattered ray to continue in the same direction.
          scattered = ray(rec.p - 0.001*rec.normal, r_in.direction()); // Offset along normal
          attenuation = color(1.0, 1.0, 1.0); // No attenuation for primary rays.
          return true;
        }
    }

    void set_invis() { is_invis = true; }

  private:
    color albedo;
    double fuzz;  // Determines the fuzziness of the reflection.
    bool is_invis;
};

class dielectric : public material {
  public:
    dielectric(double index_of_refraction) : ir(index_of_refraction), is_invis(false) {}

    // Implements the scattering function for dielectric (transparent) materials.
    // Handles both reflection and refraction based on the angle of incidence.
    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
    const override {
        if (rec.is_secondary_ray || (!is_invis)) {
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
        else {
          // For primary rays, simulate passing through by continuing the ray unchanged.
          // This means setting the scattered ray to continue in the same direction.
          scattered = ray(rec.p - 0.001*rec.normal, r_in.direction()); // Offset along normal
          attenuation = color(1.0, 1.0, 1.0); // No attenuation for primary rays.
          return true;
        }
    }

    void set_invis() { is_invis = true; }

  private:
    double ir; // Index of Refraction for the material.

    // Calculates the reflectance using Schlick's approximation.
    static double reflectance(double cosine, double ref_idx) {
        auto r0 = (1-ref_idx) / (1+ref_idx);
        r0 = r0*r0;
        return r0 + (1-r0)*pow((1 - cosine),5);
    }

    bool is_invis;
};

class diffuse_light : public material {
  public:
    diffuse_light(shared_ptr<texture> a) : emit(a) {}
    diffuse_light(color c) : emit(make_shared<solid_color>(c)) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
    const override {
        return false;
    }

    color emitted(double u, double v, const point3& p) const override {
        return emit->value(u, v, p);
    }

  private:
    shared_ptr<texture> emit;
};

#endif
