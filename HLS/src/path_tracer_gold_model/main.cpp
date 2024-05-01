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

// #include "rtweekend.h"

// #include "camera.h"
// #include "color.h"
// #include "material.h"
// #include "quad.h"
// #include "fixedpoint.h"

void cornell_w_sphere() {
    // std::vector<shared_ptr<quad>> world;

    // auto red   = (cpp_vec3(.65, .05, .05));
    // auto white = (cpp_vec3(.73, .73, .73));
    // auto green = (cpp_vec3(.12, .45, .15));
    // auto light = (cpp_vec3(15, 15, 15));
    // auto blue = (cpp_vec3(.05, .05, .65));

    // world.push_back(make_shared<quad>(point3(555,0,0), cpp_vec3(0,555,0), cpp_vec3(0,0,555), 0, false, green));
    // world.push_back(make_shared<quad>(point3(0,0,0), cpp_vec3(0,555,0), cpp_vec3(0,0,555), 0, false, red));
    // world.push_back(make_shared<quad>(point3(343, 554, 332), cpp_vec3(-130,0,0), cpp_vec3(0,0,-105), 1, false, light));
    // world.push_back(make_shared<quad>(point3(0,0,0), cpp_vec3(555,0,0), cpp_vec3(0,0,555), 0, false, white));
    // world.push_back(make_shared<quad>(point3(555,555,555), cpp_vec3(-555,0,0), cpp_vec3(0,0,-555), 0, false, white));
    // world.push_back(make_shared<quad>(point3(0,0,555), cpp_vec3(555,0,0), cpp_vec3(0,555,0), 0, false, white));
    // //invisible wall
    // world.push_back(make_shared<quad>(point3(0, 0, 0), cpp_vec3(0,555,0), cpp_vec3(555,0,0), 0, true, blue));
    // box(point3(265,0,275), point3(430,330,420), 0, white, 14, world);
    // box(point3(105,0,85), point3(260,165,235), 0, white, -18, world);

    // camera cam;

    // cam.aspect_ratio      = 1.0;
    // cam.image_width       = 240;
    // cam.samples_per_pixel = 128;
    // cam.max_depth         = 8;
    // cam.background        = cpp_vec3(0,0,0);

    // cam.vfov     = 40;
    // cam.lookfrom = point3(278, 278, -800);
    // cam.lookat   = point3(278, 278, 0);
    // cam.vup      = cpp_vec3(0,1,0);

    // cam.render(world);
}



int main() {
    cornell_w_sphere();
    return 0;
}