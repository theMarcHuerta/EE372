#ifndef MEMCONTROLLER_H
#define MEMCONTROLLER_H

#include "RTcore.h"
#include "ray.h"
#include "SphHit.h"
#include "QuadHit.h"

class MemController
{
public:
    MemController(){}
    
    #pragma hls_design interface
    void run(ac_channel<sphere_hittable> &spheres_in,
             ac_channel<quad_hittable> &quads_in, 
             ac_channel<ray<sfp_11_22>> &ray_in,
             ac_channel<buffer_obj_count> &obj_count_in,
             ac_channel<sphere_hittable> &sphere_out1,
             ac_channel<sphere_hittable> &sphere_out2,
             ac_channel<sphere_hittable> &sphere_out3,
             ac_channel<sphere_hittable> &sphere_out4,
             ac_channel<sphere_hittable> &sphere_out5,
             ac_channel<sphere_hittable> &sphere_out6,
             ac_channel<sphere_hittable> &sphere_out7,
             ac_channel<sphere_hittable> &sphere_out8,
             ac_channel<quad_hittable> &quad_out1, 
             ac_channel<quad_hittable> &quad_out2, 
             ac_channel<quad_hittable> &quad_out3, 
             ac_channel<quad_hittable> &quad_out4, 
             ac_channel<quad_hittable> &quad_out5, 
             ac_channel<quad_hittable> &quad_out6, 
             ac_channel<quad_hittable> &quad_out7, 
             ac_channel<quad_hittable> &quad_out8, 
             ac_channel<buffer_obj_count> &obj_count_out,
             ac_channel<ray<sfp_11_22>> &ray_out
            )
    {
        sphere_hittable tmp_sphere;
        quad_hittable tmp_quad;
        ray<sfp_11_22> tmp_ray;
        buffer_obj_count tmp_obj_count;

        tmp_sphere = spheres_in.read();
        tmp_quad = quads_in.read();
        tmp_ray = ray_in.read();
        tmp_obj_count = obj_count_in.read();

        ray_out.write(tmp_ray);

        obj_count_out.write(tmp_obj_count); // write idxs adn params\

        sphere_out1.write(tmp_sphere);
        quad_out1.write(tmp_quad);

        sphere_out2.write(tmp_sphere);
        quad_out2.write(tmp_quad);

        sphere_out3.write(tmp_sphere);
        quad_out3.write(tmp_quad);

        sphere_out4.write(tmp_sphere);
        quad_out4.write(tmp_quad);

        sphere_out5.write(tmp_sphere);
        quad_out5.write(tmp_quad);

        sphere_out6.write(tmp_sphere);
        quad_out6.write(tmp_quad);

        sphere_out7.write(tmp_sphere);
        quad_out7.write(tmp_quad);

        sphere_out8.write(tmp_sphere);
        quad_out8.write(tmp_quad);
    }

};

#endif
