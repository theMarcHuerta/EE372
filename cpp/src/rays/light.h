
#pragma once

#include <variant>

#include "../lib/mathlib.h"
#include "../lib/spectrum.h"

#include "samplers.h"

namespace PT {

struct Light_Sample {

    Spectrum radiance; // Radiance from the light
    Vec3 direction;    // direction to light
    float distance;    // distance to light from starting point
    float pdf;         // probability density of sample

    void transform(const Mat4& T) {
        direction = T.rotate(direction);
    }
};

struct Point_Light {

    Point_Light() : radiance(Spectrum()), sampler(Vec3(0.0f)) {}
    Point_Light(Spectrum r) : radiance(r), sampler(Vec3(0.0f)) {
    }

    Light_Sample sample(Vec3 from) const;

    Spectrum radiance;
    Samplers::Point sampler;
};

class Light {
public:
    Light(Spectrum r, unsigned int id, const Mat4& T = Mat4::I) {
        Mat4 trans = T;
        itrans = T.inverse();
        _id = id;
        has_trans = trans != Mat4::I;
        l = Point_Light(r);
    }


    Light(const Light& src) = delete;
    Light& operator=(const Light& src) = delete;
    Light& operator=(Light&& src) = default;
    Light(Light&& src) = default;

    Light_Sample sample(Vec3 from) const {
        if(has_trans) from = itrans * from;
        Light_Sample ret = l.sample(from);
        if(has_trans) ret.transform(trans);
        return ret;
    }

    bool is_discrete() const {
        return true;
    }

    unsigned int id() const {
        return _id;
    }
    void set_trans(const Mat4& T) {
        trans = T;
        itrans = T.inverse();
        has_trans = trans != Mat4::I;
    }

private:
    bool has_trans;
    Mat4 trans, itrans;
    unsigned int _id;
    Point_Light l;
};

} // namespace PT
