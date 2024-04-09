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
#include "bvh.h"


void two_spheres() {
    hittable_list world;
    auto material1 = make_shared<dielectric>(1.5);
    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    
    world.add(make_shared<sphere>(point3(0,-10, 0), 10, material1));
    world.add(make_shared<sphere>(point3(0, 10, 0), 10, material3));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.background        = color(0.70, 0.80, 1.00);

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.render(world);
}


void cornell_w_sphere() {
    hittable_list world;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));
    //invisible wall
    auto invisible_mat = make_shared<lambertian>(color(.05, .05, .65));
    invisible_mat->set_invis();

    world.add(make_shared<quad>(point3(555,0,0), vec3(0,555,0), vec3(0,0,555), green));
    world.add(make_shared<quad>(point3(0,0,0), vec3(0,555,0), vec3(0,0,555), red));
    world.add(make_shared<quad>(point3(343, 554, 332), vec3(-130,0,0), vec3(0,0,-105), light));
    world.add(make_shared<quad>(point3(0,0,0), vec3(555,0,0), vec3(0,0,555), white));
    world.add(make_shared<quad>(point3(555,555,555), vec3(-555,0,0), vec3(0,0,-555), white));
    world.add(make_shared<quad>(point3(0,0,555), vec3(555,0,0), vec3(0,555,0), white));
    //invisible metal wall
    world.add(make_shared<quad>(point3(0, 0, 0), vec3(0,555,0), vec3(555,0,0), invisible_mat));


    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0);
    world.add(make_shared<sphere>(point3(265 + 82.5, 430, 295 + 82.5), 100.0, material3));

    auto material1 = make_shared<dielectric>(1.33);
    world.add(make_shared<sphere>(point3(182, 240, 148), 75.0, material1));

    shared_ptr<hittable> box1 = box(point3(0,0,0), point3(165,330,165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265,0,295));
    world.add(box1);

    shared_ptr<hittable> box2 = box(point3(0,0,0), point3(165,165,165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130,0,65));
    world.add(box2);

    world = hittable_list(make_shared<bvh_node>(world));

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 480;
    cam.samples_per_pixel = 64;
    cam.max_depth         = 8;
    cam.background        = color(0,0,0);

    cam.vfov     = 40;
    cam.lookfrom = point3(278, 278, -800);
    cam.lookat   = point3(278, 278, 0);
    cam.vup      = vec3(0,1,0);

    cam.render(world);
}

int main() {
    switch (2) {
        case 1:  two_spheres();        break;
        case 2:  cornell_w_sphere();   break;
    }
}