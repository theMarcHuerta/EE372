
#include "../rays/pathtracer.h"
#include "../rays/samplers.h"
#include "../util/rand.h"

namespace PT {

// Return the radiance along a ray entering the camera and landing on a
// point within pixel (x,y) of the output image.
//
Spectrum Pathtracer::trace_pixel(size_t x, size_t y) {

    Vec2 xy((float)x, (float)y);
    Vec2 wh((float)out_w, (float)out_h);

    // TODO (PathTracer): Task 1

    // Generate a sample within the pixel with coordinates xy and return the
    // incoming light using trace_ray.

    // If n_samples is 1, please send the ray through the center of the pixel.
    // If n_samples > 1, please send the ray through any random point within the pixel

    // Tip: consider making a call to Samplers::Rect::Uniform

    // create a rect sampler - we can set the size of a pixel here
    Samplers::Rect::Uniform uni_rect_sampler(Vec2(1.0f, 1.0f));
    float pdf = 1.0;

    if (n_samples == 1) {
        // get pixel center
        xy += (uni_rect_sampler.size * 0.5f);
    } else {
        // get random coordinate within pixel
        Vec2 rand_vec = uni_rect_sampler.sample(pdf);
        // add these random values to our pixel sample position
        xy += rand_vec;
    }

    // compute normalized pixel coordinates
    Vec2 norm_xy = (xy / wh);

    // pass to camera
    Ray out = camera.generate_ray(norm_xy);

    return trace_ray(out);
}

Spectrum Pathtracer::trace_ray(const Ray& ray) {

    // Trace ray into scene. If nothing is hit, return default spectrum
    Trace hit = scene.hit(ray);
    if(!hit.hit) {
        return {};
    }

    // If we're using a two-sided material, treat back-faces the same as front-faces
    const BSDF& bsdf = materials[hit.material];
    if(!bsdf.is_sided() && dot(hit.normal, ray.dir) > 0.0f) {
        hit.normal = -hit.normal;
    }

    // Set up a coordinate frame at the hit point, where the surface normal becomes {0, 1, 0}
    // This gives us out_dir and later in_dir in object space, where computations involving the
    // normal become much easier. For example, cos(theta) = dot(N,dir) = dir.y!
    Mat4 object_to_world = Mat4::rotate_to(hit.normal);
    Mat4 world_to_object = object_to_world.T();
    Vec3 out_dir = world_to_object.rotate(ray.point - hit.position).unit();

    // Now we can compute the rendering equation at this point.
    // We split it into two stages:
    //  1. sampling direct lighting (i.e. directly connecting the current path to
    //     each light in the scene)
    //  2. sampling the BSDF to create a new path segment

    Spectrum radiance_out = Spectrum(0.0f, 0.0f, 0.0f);
    {

        // lambda function to sample a light. Called in loop below.
        auto sample_light = [&](const auto& light) {
            // we are only using point lights, so the light is always discrete,
            // meaning we only need one sample per light
            int samples = 1;
            for(int i = 0; i < samples; i++) {

                // Grab a sample of the light source. See rays/light.h for definition of this struct.
                // Most importantly for Task 4, it contains the distance to the light from hit.position. 
                Light_Sample sample = light.sample(hit.position);
                Vec3 in_dir = world_to_object.rotate(sample.direction);

                // If the light is below the horizon, ignore it
                float cos_theta = in_dir.y;
                if(cos_theta <= 0.0f) continue;

                // If the BSDF has 0 throughput in this direction, ignore it.
                // This is another opportunity to do Russian roulette on low-throughput rays,
                // which would allow us to skip the shadow ray cast, increasing efficiency.
                Spectrum attenuation = bsdf.evaluate(out_dir, in_dir);
                if(attenuation.luma() == 0.0f) continue;

                // TODO (PathTracer): Task 4
                // Construct a shadow ray and compute whether the intersected surface is
                // in shadow. Only accumulate light if not in shadow.

                // Tip: since you're creating the shadow ray at the intersection point, it may
                // intersect the surface at time=0. Similarly, if the ray is allowed to have
                // arbitrary length, it will hit the light it was cast at. Therefore, you should
                // modify the time_bounds of your shadow ray to account for this. Using EPS_F is
                // recommended.

                // create shadow ray
                Vec3 shadow_ray_orig = hit.position + (sample.direction*EPS_F);
                Ray shadow_ray(shadow_ray_orig, sample.direction);

                // modify dist bounds to avoid self occlusion and collision with light source
                shadow_ray.dist_bounds = Vec2(EPS_F, sample.distance - EPS_F);

                // Note: that along with the typical cos_theta, pdf factors, we divide by samples.
                // This is because we're doing another monte-carlo estimate of the lighting from
                // area lights here.

                // only add to radiance if not occluded
                if (!scene.hit(shadow_ray).hit) {
                    radiance_out += (cos_theta / (samples * sample.pdf)) * sample.radiance * attenuation;
                }
            }
        };

        // If the BSDF is discrete (i.e. uses dirac deltas/if statements), then we are never
        // going to hit the exact right direction by sampling lights, so ignore them.
        if(!bsdf.is_discrete()) {
            // loop over all the lights and accumulate radiance.
            for(const auto& light : lights)
                sample_light(light);
        }
    }

    // TODO (PathTracer): Task 5
    // Compute an indirect lighting estimate using path tracing with Monte Carlo.

    // get new ray direction and add the emissive term
    BSDF_Sample bsdf_sample = bsdf.sample(out_dir, sample_idx, n_samples, ray.depth);
    radiance_out += bsdf_sample.emissive;

    // (1) Ray objects have a depth field; if it reaches max_depth, you should
    // terminate the path.
    if (ray.depth < max_depth) {

        // (3) Compute the throughput of the recursive ray. This should be the current ray's
        // throughput scaled by the BSDF attenuation, cos(theta), and BSDF sample PDF.
        // Potentially terminate the path using Russian roulette as a function of the new throughput.
        // Note that allowing the termination probability to approach 1 may cause extra speckling.
        float cos_theta = std::abs(bsdf_sample.direction.y);
        Spectrum throughput = ray.throughput * bsdf_sample.attenuation * cos_theta / bsdf_sample.pdf;

        // Potentially terminate the path using Russian roulette
        float q = 1 - (std::max(std::max(throughput.r, throughput.g), throughput.b));
        if(RNG::unit() < q) return radiance_out;
        throughput *= (1.0f/(1-q));

        // (4) Create new scene-space ray and cast it to get incoming light. As with shadow rays, you
        // should modify time_bounds so that the ray does not intersect at time = 0. Remember to
        // set the new throughput and depth values.
        Vec3 new_ray_dir = object_to_world.rotate(bsdf_sample.direction);
        Vec3 new_ray_orig = hit.position + new_ray_dir * EPS_F;
        Ray new_ray(new_ray_orig, new_ray_dir);
        Vec2 bounds(EPS_F, INFINITY);
        new_ray.throughput = throughput;
        new_ray.dist_bounds = bounds;
        new_ray.depth = ray.depth + 1;

        // Recursively trace the new ray
        Spectrum incoming_radiance = trace_ray(new_ray);
        
        // (5) Add contribution due to incoming light with proper weighting. Remember to add in
        // the BSDF sample emissive term.
        radiance_out += (throughput * incoming_radiance);
        
    }
    return radiance_out;
}

} // namespace PT
