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

#include <iostream>

class camera {
  public:
    double aspect_ratio = 1.0;  // Defines the aspect ratio of the image (width / height).
    int    image_width  = 100;  // Width of the image in pixels.
    int    samples_per_pixel = 10;   // Number of samples per pixel for anti-aliasing.
    int    max_depth         = 10;   // Max depth for ray bouncing. Limits recursion in ray_color.
    color  background;               // Scene background color

    double vfov = 90;  // Vertical field of view in degrees.
    point3 lookfrom = point3(0,0,-1);  // The position of the camera in the scene.
    point3 lookat   = point3(0,0,0);   // The point towards which the camera is directed.
    vec3   vup      = vec3(0,1,0);     // "Up" direction in the scene, used for camera orientation.

    void render(const hittable& world) {
        initialize();  // Sets up the camera based on its current settings.

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
                    pixel_color += ray_color(r, max_depth, world);  // Accumulate color.
                }
                write_color(std::cout, pixel_color, samples_per_pixel);  // Output color to PPM.
            }
        }

        std::clog << "\rDone.                 \n";  // Completion message.
    }

    void render_iterative(const hittable& world) {
        initialize();  // Sets up the camera based on its current settings.

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";  // PPM header.

        // Loop over each pixel in the image, from top to bottom.
        for (int j = 0; j < image_height; ++j) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            // Loop over each pixel in a row, from left to right.
            for (int i = 0; i < image_width; ++i) {
                color pixel_color(0,0,0);
                // Sample the pixel color multiple times and average the results for anti-aliasing.
                for (int sample = 0; sample < samples_per_pixel; ++sample) {
                    color accumulated_color(0, 0, 0);
                    color current_attenuation(1.0, 1.0, 1.0);
                    ray current_ray = get_ray(i, j);  // Generate a ray for this pixel and sample.

                    // Iteratively trace the ray up to max_depth times.
                    for (int d = 0; d < max_depth; ++d) {
                        hit_record rec;
                        if (!world.hit(current_ray, interval(0.001, infinity), rec)) {
                            // If the ray hits nothing, add background color and break.
                            accumulated_color += current_attenuation * background;
                            break;
                        }

                        ray scattered;
                        color attenuation;
                        color emitted = rec.mat->emitted(rec.u, rec.v, rec.p);
                        accumulated_color += current_attenuation * emitted;

                        rec.is_secondary_ray = d > 0;

                        if (!rec.mat->scatter(current_ray, rec, attenuation, scattered)) {
                            accumulated_color += current_attenuation * emitted;
                            break; // No further scattering, break the loop.
                        }

                        current_attenuation *= attenuation;
                        current_ray = scattered; // Update the ray to the scattered ray.
                    }

                    pixel_color += accumulated_color;  // Accumulate color.
                }
                write_color(std::cout, pixel_color, samples_per_pixel);  // Output color to PPM.
            }
        }

        std::clog << "\rDone.                 \n";  // Completion message.
    }

  private:
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

    ray get_ray(int i, int j) const {
        // Generate a ray originating from the camera to pass through the pixel (i,j).

        auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);  // Center of the pixel.
        auto pixel_sample = pixel_center + pixel_sample_square();  // Apply random sampling within the pixel for anti-aliasing.

        auto ray_origin = center;  // Ray starts at the camera's position.
        auto ray_direction = pixel_sample - ray_origin;  // Direction from camera to sampled point.

        return ray(ray_origin, ray_direction);  // Construct and return the ray.
    }

    vec3 pixel_sample_square() const {
        // Generate a random point within the square surrounding a pixel for anti-aliasing.
        auto px = -0.5 + random_double();  // Random offset in the x direction.
        auto py = -0.5 + random_double();  // Random offset in the y direction.
        return (px * pixel_delta_u) + (py * pixel_delta_v);  // Adjust by pixel size.
    }

    color ray_color(const ray& r, int depth, const hittable& world) const {
        hit_record rec;

        // Base case: if maximum depth is reached, return black (no light).
        if (depth <= 0)
            return color(0,0,0);

         // If the ray hits nothing, return the background color.
        if (!world.hit(r, interval(0.001, infinity), rec))
            return background;

        ray scattered;
        color attenuation;
        color color_from_emission = rec.mat->emitted(rec.u, rec.v, rec.p);

        rec.is_secondary_ray = depth < max_depth;

        if (!rec.mat->scatter(r, rec, attenuation, scattered))
            return color_from_emission;

        color color_from_scatter = attenuation * ray_color(scattered, depth-1, world);

        return color_from_emission + color_from_scatter;
    }
};

#endif