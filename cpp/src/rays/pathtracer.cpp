
#include "pathtracer.h"

namespace PT {

Pathtracer::Pathtracer(Vec2 screen_dim, size_t pixel_samples, size_t depth)
    : camera(screen_dim) {
    out_w = screen_dim.x;
    out_h = screen_dim.y;
    n_samples = pixel_samples;
    max_depth = depth;

    out_buf.resize(screen_dim.x);
    for (int x = 0; x < screen_dim.x; ++x) {
        out_buf[x].resize(screen_dim.y);
        for (int y = 0; y < screen_dim.y; ++y) {
            out_buf[x][y].resize(pixel_samples, Spectrum()); // Initialize each Spectrum with default value
        }
    }
}

Pathtracer::~Pathtracer() {}

void Pathtracer::render() {

    for(size_t y = 0; y < out_h; y++) {
        for(size_t x = 0; x < out_w; x++) {
            for (size_t n = 0; n < n_samples; n++) {
                set_buf(x, y, n, trace_pixel(x, y));
            }
        }
    }
}

void Pathtracer::set_buf(int x, int y, int n, Spectrum color) {
    out_buf[x][y][n] = color;
}

void Pathtracer::set_pixel (int x, int y, Spectrum color) {
    pixels[x][y] = color;
}

Spectrum Pathtracer::buf_at(int x, int y, int n) {
    return out_buf[x][y][n];
}

Spectrum Pathtracer::pixel_at(int x, int y) {
    return pixels[x][y];
}


} // namespace PT
