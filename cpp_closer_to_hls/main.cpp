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


// void two_spheres() {
//     hittable_list world;
//     auto material1 = make_shared<dielectric>(1.5);
//     auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    
//     world.add(make_shared<sphere>(point3(0,-10, 0), 10, material1));
//     world.add(make_shared<sphere>(point3(0, 10, 0), 10, material3));

//     camera cam;

//     cam.aspect_ratio      = 16.0 / 9.0;
//     cam.image_width       = 400;
//     cam.samples_per_pixel = 100;
//     cam.max_depth         = 50;
//     cam.background        = color(0.70, 0.80, 1.00);

//     cam.vfov     = 20;
//     cam.lookfrom = point3(13,2,3);
//     cam.lookat   = point3(0,0,0);
//     cam.vup      = vec3(0,1,0);

//     cam.render(world);
// }

// void random_spheres() {
//     // The world scene is a list of hittable objects.
//     hittable_list world;

//     // Create a large sphere to act as the ground.
//     auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
//     world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

//     // Populate the world with a grid of random spheres.
//     for (int a = -11; a < 11; a++) {
//         for (int b = -11; b < 11; b++) {
//             auto choose_mat = random_double();
//             point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

//             // Place a new sphere if it's sufficiently far from a reference point.
//             if ((center - point3(4, 0.2, 0)).length() > 0.9) {
//                 shared_ptr<material> sphere_material;

//                 // Choose the material type based on a random number.
//                 if (choose_mat < 0.8) {
//                     // Diffuse material with random color.
//                     auto albedo = color::random() * color::random();
//                     sphere_material = make_shared<lambertian>(albedo);
//                     world.add(make_shared<sphere>(center, 0.2, sphere_material));
//                 } else if (choose_mat < 0.95) {
//                     // Metal material with random fuzziness and albedo.
//                     auto albedo = color::random(0.5, 1);
//                     auto fuzz = random_double(0, 0.5);
//                     sphere_material = make_shared<metal>(albedo, fuzz);
//                     world.add(make_shared<sphere>(center, 0.2, sphere_material));
//                 } else {
//                     // Glass material with fixed refractive index.
//                     sphere_material = make_shared<dielectric>(1.5);
//                     world.add(make_shared<sphere>(center, 0.2, sphere_material));
//                 }
//             }
//         }
//     }

//     // Add three larger spheres (glass, diffuse, metal) to the scene.
//     auto material1 = make_shared<dielectric>(1.5);
//     world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

//     auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
//     world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

//     auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
//     world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

//     world = hittable_list(make_shared<bvh_node>(world));
//     // Configure the camera with specific parameters.
//     camera cam;
//     cam.aspect_ratio      = 16.0 / 9.0;
//     cam.image_width       = 1200;
//     cam.samples_per_pixel = 16;
//     cam.max_depth         = 3;
//     cam.background        = color(0.70, 0.80, 1.00);

//     cam.vfov     = 40;
//     cam.lookfrom = point3(13,2,3);
//     cam.lookat   = point3(0,0,0);
//     cam.vup      = vec3(0,1,0);

//     // Render the final image using the camera and the scene.
//     cam.render(world);
// }


// void quads() {
//     hittable_list world;

//     // Materials
//     auto left_red     = make_shared<metal>(color(1.0, 0.2, 0.2), 0.0);
//     auto back_green   = make_shared<lambertian>(color(0.2, 1.0, 0.2));
//     auto right_blue   = make_shared<dielectric>(1.33);
//     auto upper_orange = make_shared<lambertian>(color(1.0, 0.5, 0.0));
//     auto lower_teal   = make_shared<metal>(color(0.2, 0.8, 0.8), 1.0);

//     // Quads
//     world.add(make_shared<quad>(point3(-3,-2, 5), vec3(0, 0,-4), vec3(0, 4, 0), left_red));
//     world.add(make_shared<quad>(point3(-2,-2, 0), vec3(4, 0, 0), vec3(0, 4, 0), back_green));
//     world.add(make_shared<quad>(point3( 3,-2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right_blue));
//     world.add(make_shared<quad>(point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper_orange));
//     world.add(make_shared<quad>(point3(-2,-3, 5), vec3(4, 0, 0), vec3(0, 0,-4), lower_teal));

//     camera cam;

//     cam.aspect_ratio      = 1.0;
//     cam.image_width       = 400;
//     cam.samples_per_pixel = 100;
//     cam.max_depth         = 50;
//     cam.background        = color(0.70, 0.80, 1.00);

//     cam.vfov     = 80;
//     cam.lookfrom = point3(0,0,9);
//     cam.lookat   = point3(0,0,0);
//     cam.vup      = vec3(0,1,0);

//     cam.render(world);
// }


// void simple_light() {
//     hittable_list world;

//     auto pertext = make_shared<solid_color>(0.5,.7,0.2);
//     world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(pertext)));
//     world.add(make_shared<sphere>(point3(0,2,0), 2, make_shared<lambertian>(pertext)));

//     auto difflight = make_shared<diffuse_light>(color(1,1,1));
//     world.add(make_shared<quad>(point3(3,1,-2), vec3(2,0,0), vec3(0,2,0), difflight));

//     camera cam;

//     cam.aspect_ratio      = 16.0 / 9.0;
//     cam.image_width       = 400;
//     cam.samples_per_pixel = 3000;
//     cam.max_depth         = 100;
//     cam.background        = color(0,0,0);

//     cam.vfov     = 20;
//     cam.lookfrom = point3(26,3,6);
//     cam.lookat   = point3(0,2,0);
//     cam.vup      = vec3(0,1,0);

//     cam.render(world);
// }

// void cornell_box() {
//     hittable_list world;

//     auto red   = make_shared<lambertian>(color(.65, .05, .05));
//     auto white = make_shared<lambertian>(color(.73, .73, .73));
//     auto green = make_shared<lambertian>(color(.12, .45, .15));
//     auto light = make_shared<diffuse_light>(color(15, 15, 15));

//     world.add(make_shared<quad>(point3(555,0,0), vec3(0,555,0), vec3(0,0,555), green));
//     world.add(make_shared<quad>(point3(0,0,0), vec3(0,555,0), vec3(0,0,555), red));
//     world.add(make_shared<quad>(point3(343, 554, 332), vec3(-130,0,0), vec3(0,0,-105), light));
//     world.add(make_shared<quad>(point3(0,0,0), vec3(555,0,0), vec3(0,0,555), white));
//     world.add(make_shared<quad>(point3(555,555,555), vec3(-555,0,0), vec3(0,0,-555), white));
//     world.add(make_shared<quad>(point3(0,0,555), vec3(555,0,0), vec3(0,555,0), white));

//     world.add(box(point3(130, 0, 65), point3(295, 165, 230), white));
//     world.add(box(point3(265, 0, 295), point3(430, 330, 460), white));

//     camera cam;

//     cam.aspect_ratio      = 1.0;
//     cam.image_width       = 600;
//     cam.samples_per_pixel = 64;
//     cam.max_depth         = 3;
//     cam.background        = color(0,0,0);

//     cam.vfov     = 40;
//     cam.lookfrom = point3(278, 278, -800);
//     cam.lookat   = point3(278, 278, 0);
//     cam.vup      = vec3(0,1,0);

//     cam.render(world);
// }


// void full_cornell_box() {
//     hittable_list world;

//     auto red   = make_shared<lambertian>(color(.65, .05, .05));
//     auto white = make_shared<lambertian>(color(.73, .73, .73));
//     auto green = make_shared<lambertian>(color(.12, .45, .15));
//     auto light = make_shared<diffuse_light>(color(15, 15, 15));

//     world.add(make_shared<quad>(point3(555,0,0), vec3(0,555,0), vec3(0,0,555), green));
//     world.add(make_shared<quad>(point3(0,0,0), vec3(0,555,0), vec3(0,0,555), red));
//     world.add(make_shared<quad>(point3(343, 554, 332), vec3(-130,0,0), vec3(0,0,-105), light));
//     world.add(make_shared<quad>(point3(0,0,0), vec3(555,0,0), vec3(0,0,555), white));
//     world.add(make_shared<quad>(point3(555,555,555), vec3(-555,0,0), vec3(0,0,-555), white));
//     world.add(make_shared<quad>(point3(0,0,555), vec3(555,0,0), vec3(0,555,0), white));

//     shared_ptr<hittable> box1 = box(point3(0,0,0), point3(165,330,165), white);
//     box1 = make_shared<rotate_y>(box1, 15);
//     box1 = make_shared<translate>(box1, vec3(265,0,295));
//     world.add(box1);

//     shared_ptr<hittable> box2 = box(point3(0,0,0), point3(165,165,165), white);
//     box2 = make_shared<rotate_y>(box2, -18);
//     box2 = make_shared<translate>(box2, vec3(130,0,65));
//     world.add(box2);

//     camera cam;

//     cam.aspect_ratio      = 1.0;
//     cam.image_width       = 600;
//     cam.samples_per_pixel = 1000;
//     cam.max_depth         = 128;
//     cam.background        = color(0,0,0);

//     cam.vfov     = 40;
//     cam.lookfrom = point3(278, 278, -800);
//     cam.lookat   = point3(278, 278, 0);
//     cam.vup      = vec3(0,1,0);

//     cam.render(world);
// }


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


    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), .1);
    world.add(make_shared<sphere>(point3(265 + 82.5, 430, 295 + 82.5), 100.0, material3, false));

    auto material1 = make_shared<dielectric>(1.33);
    world.add(make_shared<sphere>(point3(182, 240, 148), 75.0, material1, false));

    shared_ptr<hittable> box1 = box(point3(0,0,0), point3(165,330,165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265,0,295));
    world.add(box1);

    shared_ptr<hittable> box2 = box(point3(0,0,0), point3(165,165,165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130,0,65));
    world.add(box2);

    // world = hittable_list(make_shared<bvh_node>(world));

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

// void cornl_mir_flr() {
//     hittable_list world;

//     auto red   = make_shared<lambertian>(color(.65, .05, .05));
//     auto white = make_shared<lambertian>(color(.73, .73, .73));
//     auto green = make_shared<lambertian>(color(.12, .45, .15));
//     auto light = make_shared<diffuse_light>(color(15, 15, 15));

//     auto light2 = make_shared<diffuse_light>(color(5, 1, 1));
//     //mirror
//     auto pruple_metal = make_shared<metal>(color(.95, .4, .75), 0.3);
//     auto black_metal = make_shared<metal>(color(.15, .05, .10), 0.05);
//     auto green_metal = make_shared<metal>(color(.12, .45, .15), 0.6);
//     //BLUE invisible wall
//     auto invisible_mat = make_shared<lambertian>(color(.05, .05, .65));
//     invisible_mat->set_invis();

//     world.add(make_shared<quad>(point3(555,0,0), vec3(0,555,0), vec3(0,0,555), green_metal));
//     world.add(make_shared<quad>(point3(0,0,0), vec3(0,555,0), vec3(0,0,555), red));
//     world.add(make_shared<quad>(point3(343, 554, 332), vec3(-130,0,0), vec3(0,0,-105), light));
//     //floor
//     world.add(make_shared<quad>(point3(0,0,0), vec3(555,0,0), vec3(0,0,555), pruple_metal));
//     //celing
//     world.add(make_shared<quad>(point3(555,555,555), vec3(-555,0,0), vec3(0,0,-555), white));
//     // backwall
//     world.add(make_shared<quad>(point3(0,0,555), vec3(555,0,0), vec3(0,555,0), white));
//     //invisible metal wall for front
//     world.add(make_shared<quad>(point3(0, 0, 0), vec3(0,555,0), vec3(555,0,0), invisible_mat));


//     auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0);
//     world.add(make_shared<sphere>(point3(265 + 82.5, 430, 295 + 82.5), 100.0, material3));

//     auto material1 = make_shared<dielectric>(1.33);
//     world.add(make_shared<sphere>(point3(182, 240, 148), 75.0, material1));

//     shared_ptr<hittable> box1 = box(point3(0,0,0), point3(165,330,165), white);
//     box1 = make_shared<rotate_y>(box1, 15);
//     box1 = make_shared<translate>(box1, vec3(265,0,295));
//     world.add(box1);

//     shared_ptr<hittable> box2 = box(point3(0,0,0), point3(165,165,165), white);
//     box2 = make_shared<rotate_y>(box2, -18);
//     box2 = make_shared<translate>(box2, vec3(130,0,65));
//     world.add(box2);
    
//     //auto material2 = make_shared<dielectric>(3);
//     //light on top of black box
//     //world.add(make_shared<quad>(point3(343, 554, 332), vec3(-130,0,0), vec3(0,0,-105), light2));
//     shared_ptr<hittable> small_light = make_shared<quad>(point3(0, 83, 0), vec3(-50,0,0), vec3(0,50,0), light2);
//     small_light = make_shared<rotate_y>(small_light, 36);
//     small_light = make_shared<translate>(small_light, vec3(423,-70,124));
//     world.add(small_light);

//     shared_ptr<hittable> box3 = box(point3(0,0,0), point3(110,82,82), black_metal);
//     box3 = make_shared<rotate_y>(box3, 36);
//     box3 = make_shared<translate>(box3, vec3(360,0,170));
//     world.add(box3);

//     world = hittable_list(make_shared<bvh_node>(world));

//     camera cam;

//     cam.aspect_ratio      = 1.0;
//     cam.image_width       = 720;
//     cam.samples_per_pixel = 8;
//     cam.max_depth         = 8;
//     cam.background        = color(0,0,0);

//     cam.vfov     = 40;
//     cam.lookfrom = point3(278, 278, -800);
//     cam.lookat   = point3(278, 278, 0);
//     cam.vup      = vec3(0,1,0);

//     cam.render(world);
// }


int main() {
    // switch (7) {
    //     case 1:  random_spheres();     break;
    //     case 2:  two_spheres();        break;
    //     case 3:  quads();              break;
    //     case 4:  simple_light();       break;
    //     case 5:  cornell_box();        break;
    //     case 6:  full_cornell_box();   break;
    //     case 7:  cornell_w_sphere();   break;
    //     case 8:  cornl_mir_flr();      break;
    // }
    cornell_w_sphere();
}