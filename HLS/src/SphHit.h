
#include <ac_int.h>
#include <ac_fixed.h>
#include <ac_channel.h>
#include <sstream>


#include "RTcore.h"
#include "ShaderCores.h"
#include "RayGeneration.h"
#include "HitRecord.h"
// Include mc_scverify.h for CCS_* macros
#include <mc_scverify.h>


class SphereHit {
public:
    // Method to determine if a ray intersects with a sphere
    #pragma hls_design ccore
    bool hit(const ray& r, sfp_11_22& closest_so_far, const sphere_hittable& sphere, HitRecord& rec) { // NOT SURE WHETHER TO AC CHANNEL THE SPHERE HITABLE OR IF THE LOOP TAKES CARE OF THIS HMMM
        full_vec3 oc = {r.origin.x - sphere.center.x, r.origin.y - sphere.center.y, r.origin.z - sphere.center.z};  // Vector from ray origin to sphere center.
        sfp_11_22 a = length_squared(r.direction); // INSERT SQUARING FUNCTION HERE
        sfp_11_22 half_b = dot(oc, r.direction);
        sfp_11_22 c = length_squared(oc) - (sphere.radius * sphere.radius);

        sfp_11_22 discriminant = half_b * half_b - a * c;
        if (discriminant < 0) return false;  // No intersection// ALSO SHOULD I MAKE THE COMPARIOSN FIXED POINT OR WHAT?

        sfp_11_22 sqrtd = ac_math::ac_sqrt<sfp_11_22>(discriminant);
        sfp_11_22 root = (-half_b - sqrtd) / a;

        if (root < 0 || root > closest_so_far) {
            root = (-half_b + sqrtd) / a;
            if (root < 0 || root > closest_so_far)
                return false;
        }

        rec.t = root;
        rec.p.x = r.origin.x + r.direction.x * root;
        rec.p.y = r.origin.y + r.direction.y * root;
        rec.p.z = r.origin.z + r.direction.z * root;
        rec.color = sphere.sph_color;

        full_vec3 outward_normal = {(rec.p.x - sphere.center.x) / sphere.radius,
                            (rec.p.y - sphere.center.y) / sphere.radius,
                            (rec.p.z - sphere.center.z) / sphere.radius};

        // bool front_face = dot(r.direction, outward_normal) < 0; // ALSO HAVE A FUNCTION FO RTHIS IN HITRECORD BUT ITS A CLASS MIGHT JUST MAKE IT A FUNCTION
        HitRecordPhysics setfacenorm; 
        // rec.normal = front_face ? outward_normal : full_vec3{-outward_normal.x, -outward_normal.y, -outward_normal.z};
        setfacenorm.setFaceNormal(rec,r, outward_normal);
        rec.mat = sphere.mat_type;

        closest_so_far = root;  // Update the closest_so_far
        return true;
    }
};