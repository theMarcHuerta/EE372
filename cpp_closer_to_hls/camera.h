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
#include <thread>
#include <vector>
#include <sstream>
#include <atomic>
#include <chrono>

class camera {
  public:
    double aspect_ratio = 1.0;  // Defines the aspect ratio of the image (width / height).
    int    image_width  = 100;  // Width of the image in pixels.
    int    samples_per_pixel = 10;   // Number of samples per pixel for anti-aliasing.
    int    max_depth         = 10;   // Max depth for ray bouncing. Limits recursion in ray_color.
    color  background;               // Scene background color
    //mt stuff
    static constexpr int num_threads = 1;
    std::atomic<int> lines_remaining;

    double vfov = 90;  // Vertical field of view in degrees.
    point3 lookfrom = point3(0,0,-1);  // The position of the camera in the scene.
    point3 lookat   = point3(0,0,0);   // The point towards which the camera is directed.
    vec3   vup      = vec3(0,1,0);     // "Up" direction in the scene, used for camera orientation.

    void render(const hittable& world) {
        initialize();
        lines_remaining = image_height; // Initialize the atomic counter with the total number of scanlines

        // std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        std::vector<std::thread> threads;
        std::vector<std::string> output(image_height); // Correctly pre-allocated output vector
        int segment_height = image_height / num_threads;

        // Spawn a thread to report progress
        std::thread progress_thread([&]() {
            while (lines_remaining > 0) {
                std::clog << "\rScanlines remaining: " << lines_remaining.load() << ' ' << std::flush;
                std::this_thread::sleep_for(std::chrono::seconds(1)); // Update progress every second
            }
        });

        for (int t = 0; t < num_threads; ++t) {
            int start_line = t * segment_height;
            int end_line = (t == num_threads - 1) ? image_height : (t + 1) * segment_height;
            threads.push_back(std::thread([=, &world, &output]() { // Capture world and output by reference
                std::ostringstream local_stream;
                for (int j = start_line; j < end_line; ++j) {
                    for (int i = 0; i < image_width; ++i) {
                        color pixel_color(0,0,0);
                        for (int sample = 0; sample < samples_per_pixel; ++sample) {
                            auto r = get_ray(i, j);
                            // std::cout << 1 << std::endl;
                            // std::cout << r.origin() << "\n";
                            // std::cout << r.direction() << "\n\n";
                            // auto tmp_color = ray_color(r, max_depth, world);
                            // std::cout << tmp_color << "\n";
                            pixel_color += ray_color(r, max_depth, world);
                        }
                        write_color(local_stream, pixel_color, samples_per_pixel);
                    }
                    lines_remaining -= 1;
                    output[j] = local_stream.str(); // Directly writing to the output vector
                    local_stream.str("");
                    local_stream.clear();
                }
                int lines_processed = end_line - start_line;
                lines_remaining -= lines_processed; // Decrement once after processing a segment
            }));
        }

        for (auto& thread : threads) {
            thread.join();
        }

        progress_thread.join();
        std::clog << "\rDone.                                  \n";

        // // Output the combined results
        // for (const auto& line : output) {
        //     std::cout << line;
        // }
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

        // std::cout << center << "\n";
        // std::cout << pixel00_loc << "\n\n";
        // std::cout << pixel_delta_u << "\n";
        // std::cout << pixel_delta_v << "\n\n";
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

    color ray_color(const ray& r, int max_depth, const hittable& world) const {
        color accumulated_color(0, 0, 0); // Initialize accumulated color as black
        color current_attenuation(1, 1, 1); // Start with no attenuation
        ray current_ray = r; // The current ray starts as the initial ray

        for(int depth = 0; depth < max_depth; ++depth) {
            hit_record rec;

            current_ray.first_ray = depth == 0 ? true : false;
            // Check if the ray hits the background
            if (!world.hit(current_ray, interval(0.001, 2048), rec)) {
                // std::cout << 1 << "\n";
                
                // std::cout << 0 << " " << 0 << " " << 0 << "\n";
                // std::cout << 0 << " " << 0 << " " << 0 << "\n";
                // std::cout << 0 << "\n";
                // std::cout << 0 << "\n";
                // std::cout << 0 << "\n";
                // std::cout << 0 << "\n";
                // std::cout << 0 << "\n";
                // std::cout << 0 << " " << 0 << " " << 0 << "\n\n";
                // std::cout << 0 << "\n";
                // std::cout << 0 << "\n\n";
                accumulated_color += current_attenuation * background; // Apply background color
                break;
            }
            // std::cout << 1 << "\n";
            // std::cout << rec.p << "\n";
            // std::cout << rec.normal << "\n";
            // std::cout << rec.front_face << "\n";
            // std::cout << rec.t << "\n";
            // std::cout << rec.u << "\n";
            // std::cout << rec.v << "\n";
            // std::cout << rec.mat->matnum() << "\n";
            // std::cout << rec.mat->colorofmat() << "\n\n";

            // After the first iteration, any ray is considered a secondary ray
            rec.is_secondary_ray = depth > 0;

            // Emitted light from the material (if any)
            color emitted = rec.mat->emitted(rec.u, rec.v, rec.p);
            accumulated_color += current_attenuation * emitted; // Add emitted light to accumulated color

            ray scattered; // The ray that results from scattering
            color attenuation; // The attenuation of the ray after scattering

            // If the material scatters the ray, update the current attenuation and the current ray
            if (rec.mat->scatter(current_ray, rec, attenuation, scattered)) {
                current_attenuation = current_attenuation * attenuation; // Update attenuation
                // current_ray = scattered;
                point3 tmp_p = scattered.origin();
                if (std::abs(rec.p.x()) < 0.0009765625) {tmp_p.e[0] = rec.p.x() < 0 ? -0.0009765625 : 0.0009765625;}
                if (std::abs(rec.p.y()) < 0.0009765625) {tmp_p.e[1] = rec.p.y() < 0 ? -0.0009765625 : 0.0009765625;}
                if (std::abs(rec.p.z()) < 0.0009765625) {tmp_p.e[2] = rec.p.z() < 0 ? -0.0009765625 : 0.0009765625;}
                current_ray = ray(tmp_p, scattered.direction()); // The scattered ray becomes the current ray for the next iteration
                if (depth==0){
                std::cout << current_ray.origin() << "\n";
                std::cout << current_ray.direction() << "\n\n";
                }
            } else {
                // If the material does not scatter the ray, no further color contributions are expected
                // if (depth==7){
                // std::cout << 0 << "\n";
                // std::cout << 0 << "\n\n";
                // }
                break;
            }
        }

        return accumulated_color;
    }
};

#endif