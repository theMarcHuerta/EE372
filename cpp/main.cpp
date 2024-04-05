/*
How Everything Connects and Works Together
Scene Construction: The scene, represented by world, is a collection of objects that can be hit by rays. 
It's constructed using hittable_list, which allows adding any object that implements the hittable interface.
Here, we add spheres with various materials to the scene.

Materials: Each sphere is assigned a material (lambertian for diffuse, metal for reflective, and dielectric 
for refractive materials). The material determines how rays interact with the object's surface, affecting 
the color and appearance of objects.

Camera Setup: The camera is configured with parameters like aspect ratio, image width, samples per pixel 
(for anti-aliasing), and the maximum recursion depth for rays. The lookfrom, lookat, and vup vectors define 
the camera's position and orientation in the scene.

Rendering: The cam.render(world) call kicks off the rendering process. For each pixel in the output image, 
multiple rays may be sampled (depending on samples_per_pixel) to calculate color through a recursive process 
that simulates light bouncing around the scene (max_depth controls recursion depth). This process involves 
shooting rays from the camera into the scene, determining ray-object intersections using the hit function of 
each object, and using the material properties to calculate the color contribution of each ray.
*/

#include "rtweekend.h"

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

int main() {
    // The world scene is a list of hittable objects.
    hittable_list world;

    // Create a large sphere to act as the ground.
    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    // Populate the world with a grid of random spheres.
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            // Place a new sphere if it's sufficiently far from a reference point.
            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                // Choose the material type based on a random number.
                if (choose_mat < 0.8) {
                    // Diffuse material with random color.
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // Metal material with random fuzziness and albedo.
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // Glass material with fixed refractive index.
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    // Add three larger spheres (glass, diffuse, metal) to the scene.
    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    // Configure the camera with specific parameters.
    camera cam;
    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 1200;
    cam.samples_per_pixel = 16;
    cam.max_depth         = 3;

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    // Render the final image using the camera and the scene.
    cam.render(world);
}
