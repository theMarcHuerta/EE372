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
#include "material.h"
#include "quad.h"
// #include "fixedpoint.h"

// Rotation function around the y-axis for vectors
vec3 rotate_y(const vec3& v, double sin_theta, double cos_theta) {
    return vec3(cos_theta * v.x() + sin_theta * v.z(), v.y(), -sin_theta * v.x() + cos_theta * v.z());
}

inline void box(const point3& a, const point3& b, int mat, color mat_color, double angle_degrees, std::vector<shared_ptr<quad>>& world)
{
    auto min = point3(fmin(a.x(), b.x()), fmin(a.y(), b.y()), fmin(a.z(), b.z()));
    auto max = point3(fmax(a.x(), b.x()), fmax(a.y(), b.y()), fmax(a.z(), b.z()));

    // Center of the box (only x and z are used for calculating the rotation)
    point3 center = 0.5 * (min + max);
    vec3 center_horizontal(center.x(), 0, center.z());

    // Vectors defining the box dimensions
    vec3 dx = vec3(max.x() - min.x(), 0, 0);
    vec3 dy = vec3(0, max.y() - min.y(), 0);
    vec3 dz = vec3(0, 0, max.z() - min.z());

    // Calculate sine and cosine for the rotation
    double radians = angle_degrees * pi / 180.0;
    double cos_theta = cos(radians);
    double sin_theta = sin(radians);

    // Rotate vectors
    vec3 rotated_dx = rotate_y(dx, sin_theta, cos_theta);
    vec3 rotated_dz = rotate_y(dz, sin_theta, cos_theta);

    // Apply rotation around the center by adjusting corner positions
    // Start at the bottom front left corner and rotate around the horizontal center
    point3 front_bottom_left = center_horizontal + rotate_y(min - center_horizontal, sin_theta, cos_theta);
    point3 front_bottom_right = front_bottom_left + rotated_dx;

    // Construct quads with rotated vectors and adjusted positions
    world.push_back(make_shared<quad>(front_bottom_left, rotated_dx, dy, mat, false, mat_color)); // Front
    world.push_back(make_shared<quad>(front_bottom_left, rotated_dx, dy, mat, false, mat_color)); // Back
    world.push_back(make_shared<quad>(front_bottom_right, rotated_dz, dy, mat, false, mat_color)); // Right
    world.push_back(make_shared<quad>(front_bottom_left, rotated_dz, dy, mat, false, mat_color)); // Left
    world.push_back(make_shared<quad>(front_bottom_left + dy, rotated_dx, rotated_dz, mat, false, mat_color)); // Top

}

void cornell_w_sphere() {
    std::vector<shared_ptr<quad>> world;

    auto red   = (color(.65, .05, .05));
    auto white = (color(.73, .73, .73));
    auto green = (color(.12, .45, .15));
    auto light = (color(15, 15, 15));
    auto blue = (color(.05, .05, .65));

    world.push_back(make_shared<quad>(point3(555,0,0), vec3(0,555,0), vec3(0,0,555), 0, false, green));
    world.push_back(make_shared<quad>(point3(0,0,0), vec3(0,555,0), vec3(0,0,555), 0, false, red));
    world.push_back(make_shared<quad>(point3(343, 554, 332), vec3(-130,0,0), vec3(0,0,-105), 1, false, light));
    world.push_back(make_shared<quad>(point3(0,0,0), vec3(555,0,0), vec3(0,0,555), 0, false, white));
    world.push_back(make_shared<quad>(point3(555,555,555), vec3(-555,0,0), vec3(0,0,-555), 0, false, white));
    world.push_back(make_shared<quad>(point3(0,0,555), vec3(555,0,0), vec3(0,555,0), 0, false, white));
    //invisible wall
    world.push_back(make_shared<quad>(point3(0, 0, 0), vec3(0,555,0), vec3(555,0,0), 0, true, blue));
    box(point3(265,0,275), point3(430,330,420), 0, white, 14, world);
    box(point3(105,0,85), point3(260,165,235), 0, white, -18, world);

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 240;
    cam.samples_per_pixel = 128;
    cam.max_depth         = 8;
    cam.background        = color(0,0,0);

    cam.vfov     = 40;
    cam.lookfrom = point3(278, 278, -800);
    cam.lookat   = point3(278, 278, 0);
    cam.vup      = vec3(0,1,0);

    cam.render(world);
}



int main() {
    cornell_w_sphere();
    return 0;
}