
#include "../rays/bsdf.h"
#include "../util/rand.h"

namespace PT {

Vec3 reflect(Vec3 dir) {

    // TODO (PathTracer): Task 6
    // Return reflection of dir about the surface normal (0,1,0).

    // convert dir to object space
    // wo = -wi + 2*dot(wi, n)*n
    // dot(wi, n) = wi.y
    Vec3 surface_norm(0.0f, 1.0f, 0.0f);
    Vec3 reflect_dir = -dir + (2*dir.y*surface_norm);
    return reflect_dir;
}

Vec3 reflect(Vec3 out_dir, Vec3 n) {
    return -out_dir + 2 * dot(out_dir, n) * n;
}

BSDF_Sample BSDF_Lambertian::sample(Vec3 out_dir, size_t sample_idx, size_t total_samples, size_t depth) const {

    // TODO (PathTracer): Task 5
    // Implement lambertian BSDF. Use of BSDF_Lambertian::sampler may be useful
    BSDF_Sample ret;
    if (depth == 0) 
        ret.direction = sampler.stratifiedSample(ret.pdf, sample_idx, total_samples);
    else 
        ret.direction = sampler.sample(ret.pdf);
    ret.attenuation = albedo * (1.0f / PI_F); // What is the ratio of reflected/incoming light?
    return ret;
}

Spectrum BSDF_Lambertian::evaluate(Vec3 out_dir, Vec3 in_dir) const {
    return albedo * (1.0f / PI_F);
}

BSDF_Sample BSDF_Mirror::sample(Vec3 out_dir, size_t sample_idx, size_t total_samples, size_t depth) const {

    // TODO (PathTracer): Task 6
    // Implement mirror BSDF

    BSDF_Sample ret;
    // for a perfect mirror, all light is reflected
    ret.attenuation = Spectrum(1.0f); 
    // direction is the direction calculated by our reflect function
    ret.direction = reflect(out_dir);
    // only one possible direction, pmf = 1
    ret.pdf = 1.0f; 
    return ret;
}

Spectrum BSDF_Mirror::evaluate(Vec3 out_dir, Vec3 in_dir) const {
    // Technically, we would return the proper reflectance
    // if in_dir was the perfectly reflected out_dir, but given
    // that we assume these are single exact directions in a
    // continuous space, just assume that we never hit them
    // _exactly_ and always return 0.
    return Spectrum(0.0f);
}

BSDF_Sample BSDF_Diffuse::sample(Vec3 out_dir, size_t sample_idx, size_t total_samples, size_t depth) const {
    BSDF_Sample ret;
    if (depth == 0) 
        ret.direction = sampler.stratifiedSample(ret.pdf, sample_idx, total_samples);
    else 
        ret.direction = sampler.sample(ret.pdf);
    ret.emissive = radiance;
    ret.attenuation = {};
    return ret;
}

Spectrum BSDF_Diffuse::evaluate(Vec3 out_dir, Vec3 in_dir) const {
    // No incoming light is reflected; only emitted
    return {};
}

} // namespace PT
