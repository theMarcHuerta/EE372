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
#include "quad.h"
#include "fixedpoint.h"


void cornell_w_sphere() {
    hittable_list world;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));
    //invisible wall
    auto invisible_mat = make_shared<lambertian>(color(.05, .05, .65));

    world.add(make_shared<quad>(point3(555,0,0), vec3(0,555,0), vec3(0,0,555), green, false));
    world.add(make_shared<quad>(point3(0,0,0), vec3(0,555,0), vec3(0,0,555), red, false));
    world.add(make_shared<quad>(point3(343, 554, 332), vec3(-130,0,0), vec3(0,0,-105), light, false));
    world.add(make_shared<quad>(point3(0,0,0), vec3(555,0,0), vec3(0,0,555), white, false));
    world.add(make_shared<quad>(point3(555,555,555), vec3(-555,0,0), vec3(0,0,-555), white, false));
    world.add(make_shared<quad>(point3(0,0,555), vec3(555,0,0), vec3(0,555,0), white, false));
    //invisible metal wall
    world.add(make_shared<quad>(point3(0, 0, 0), vec3(0,555,0), vec3(555,0,0), invisible_mat, true));


    // auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    // auto blue_metal = make_shared<metal>(color(.3, .4, .7), 0.6);
    // world.add(make_shared<sphere>(point3(265 + 82.5, 430, 295 + 82.5), 100.0, material3, false));

    // auto material1 = make_shared<dielectric>(1.33);
    // world.add(make_shared<sphere>(point3(182, 240, 148), 75.0, blue_metal, false));

    shared_ptr<hittable> box1 = box(point3(265,0,275), point3(430,330,420), white, 14);
    // shared_ptr<hittable> box1 = box(point3(0,0,0), point3(165,330,165), white);
    // box1 = make_shared<rotate_y>(box1, 15);
    // box1 = make_shared<translate>(box1, vec3(265,0,295));
    world.add(box1);

    shared_ptr<hittable> box2 = box(point3(105,0,85), point3(260,165,235), white, -18);
    // shared_ptr<hittable> box2 = box(point3(0,0,0), point3(165,165,165), white);
    // box2 = make_shared<rotate_y>(box2, -18);
    // box2 = make_shared<translate>(box2, vec3(130,0,65));
    world.add(box2);

    // world = hittable_list(make_shared<bvh_node>(world));

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 240;
    cam.samples_per_pixel = 1;
    cam.max_depth         = 1;
    cam.background        = color(0,0,0);

    cam.vfov     = 40;
    cam.lookfrom = point3(278, 278, -800);
    cam.lookat   = point3(278, 278, 0);
    cam.vup      = vec3(0,1,0);

    cam.render(world);
}



int main() {
    cornell_w_sphere();
    // FixedPoint a22 = FixedPoint(0.01);
    // std::cout << a22.toDouble() << std::endl;
    // // FixedPoint b22 = FixedPoint(2.0);
    // // FixedPoint c22 = a22 / b22;
    // // std::cout << "a22 / b22 = " << c22.toDouble() << std::endl;

    // // FixedPoint a21 = FixedPoint(3.5);
    // // FixedPoint b21 = FixedPoint(2.25);
    // // FixedPoint c21 = a21 + b21;
    // // FixedPoint d21 = a21 * b21;
    // // std::cout << "a21 + b21 = " << c21.toDouble() << std::endl;
    // // std::cout << "a21 * b21 = " << d21.toDouble() << std::endl;

    // // FixedPoint a20 = FixedPoint(.25);
    // // FixedPoint b20 = FixedPoint(.25);
    // // FixedPoint d20 = a20 * b20;
    // // std::cout << "a20 * b20 = " << d20.toDouble() << std::endl;
    return 0;
}