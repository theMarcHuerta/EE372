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

#include "rtweekend.h"

#include "color.h"
#include "hittable.h"
#include "vec3.h"
#include "material.h"
#include "quad.h"

#include <iostream>
#include <thread>
#include <vector>
#include <sstream>
#include <atomic>
#include <chrono>
#define LONGEST_DISTANCE 2896

class camera {
  public:
    double aspect_ratio = 1.0;  // Defines the aspect ratio of the image (width / height).
    int    image_width  = 100;  // Width of the image in pixels.
    int    samples_per_pixel = 10;   // Number of samples per pixel for anti-aliasing.
    int    max_depth         = 10;   // Max depth for ray bouncing. Limits recursion in ray_color.
    color  background;               // Scene background color
    //mt stuff
    double vfov = 90;  // Vertical field of view in degrees.
    point3 lookfrom = point3(0,0,-1);  // The position of the camera in the scene.
    point3 lookat   = point3(0,0,0);   // The point towards which the camera is directed.
    vec3   vup      = vec3(0,1,0);     // "Up" direction in the scene, used for camera orientation.

    vec3 pixel_sample_square() const {
        // Generate a random point within the square surrounding a pixel for anti-aliasing.
        auto px = -0.5 + random_double1();  // Random offset in the x direction.
        auto py = -0.5 + random_double2();  // Random offset in the y direction.
        return (px * pixel_delta_u) + (py * pixel_delta_v);  // Adjust by pixel size.
    }

    ray get_ray(int i, int j) const {
        // Generate a ray originating from the camera to pass through the pixel (i,j).

        auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);  // Center of the pixel.
        auto pixel_sample = pixel_center + pixel_sample_square();  // Apply random sampling within the pixel for anti-aliasing.

        auto ray_origin = center;  // Ray starts at the camera's position.
        auto ray_direction = pixel_sample - ray_origin;  // Direction from camera to sampled point.

        return ray(ray_origin, ray_direction);  // Construct and return the ray.
    }

    // Overrides hit method to check all objects in the list for the closest hit.
    bool WorldHit(const std::vector<shared_ptr<quad>>& objects, const ray& r, hit_record& rec) const {
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

    color ray_color(const ray& r, int max_depth, const std::vector<shared_ptr<quad>>& world) const {
        color accumulated_color(0, 0, 0); // Initialize accumulated color as black
        color current_attenuation(1, 1, 1); // Start with no attenuation
        ray current_ray = r; // The current ray starts as the initial ray

        for(int depth = 0; depth < max_depth; ++depth) {
            hit_record rec;

            current_ray.first_ray = depth == 0 ? true : false;
            // Check if the ray hits the background
            if (!WorldHit(world, current_ray, rec)) {
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
                color emitted = rec_tmp.mat_color;
                accumulated_color += current_attenuation * emitted; // Add emitted light to accumulated color
                break;
            }
            else {
                lambertian scatterfunc(rec.mat_color);
                ray scattered; // The ray that results from scattering
                color attenuation; // The attenuation of the ray after scattering
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
                color pixel_color(0,0,0);
                // Sample the pixel color multiple times and average the results for anti-aliasing.
                for (int sample = 0; sample < samples_per_pixel; ++sample) {
                    ray r = get_ray(i, j);  // Generate a ray for this pixel and sample.
                    r = ray(r.origin(), vec3(FixedPoint<23>(r.direction().x()).toDouble(), FixedPoint<23>(r.direction().y()).toDouble(), FixedPoint<23>(r.direction().z()).toDouble()));
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
    vec3   pixel_delta_u;  // Vector to move one pixel to the right on the image plane.
    vec3   pixel_delta_v;  // Vector to move one pixel down on the image plane.
    vec3   u, v, w;        // Orthogonal basis vectors for the camera orientation.

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
        vec3 viewport_u = viewport_width * u;  // Vector spanning the viewport width.
        vec3 viewport_v = viewport_height * -v;  // Vector spanning the viewport height, inverted.

        // Calculate pixel size in world units on the viewport.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the world space position of the top-left pixel (0,0).
        auto viewport_upper_left = center - (focal_length * w) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

};

#endif