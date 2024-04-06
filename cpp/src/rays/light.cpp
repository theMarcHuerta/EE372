
#include "light.h"

namespace PT {

Light_Sample Point_Light::sample(Vec3 from) const {
    Light_Sample ret;
    ret.direction = -from.unit();
    ret.distance = from.norm();
    ret.pdf = 1.0f;
    ret.radiance = radiance;
    return ret;
}

} // namespace PT
