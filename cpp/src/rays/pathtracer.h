
#pragma once

#include <unordered_map>

#include "../util/camera.h"
#include "../lib/mathlib.h"
#include "../rays/trace.h"
#include "bsdf.h"
#include "light.h"
#include "bvh.h"
#include "tri_mesh.h"

namespace PT {

class Pathtracer {
public:
    Pathtracer(Vec2 screen_dim, size_t pixel_samples, size_t depth);
    ~Pathtracer();

    void set_buf(int x, int y, int n, Spectrum color);
    void set_pixel (int x, int y, Spectrum color);
    Spectrum buf_at(int x, int y, int n);
    Spectrum pixel_at(int x, int y);

private:
    // Internal
    void render();

    /// Relevant to student
    Spectrum trace_pixel(size_t x, size_t y);
    Spectrum trace_ray(const Ray& ray);

    BVH<Triangle> scene;
    std::vector<Light> lights;
    std::vector<BSDF> materials;
    std::unordered_map<unsigned int, size_t> mat_cache;

    Camera camera;
    size_t out_w, out_h, n_samples, max_depth;

    std::vector<std::vector<std::vector<Spectrum>>> out_buf;
    std::vector<std::vector<Spectrum>> pixels;
};

} // namespace PT
