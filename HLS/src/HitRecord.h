#ifndef _GLOBAL_SIMPLE_H
#define _GLOBAL_SIMPLE_H

#include <ac_int.h>
#include <ac_fixed.h>
#include <ac_channel.h>
#include <sstream>


#include "RTcore.h"
#include "ShaderCores.h"
#include "RayGeneration.h"

// Include mc_scverify.h for CCS_* macros
#include <mc_scverify.h>

struct HitRecord {
    point3 p;
    full_vec3 normal;
    bool front_face;
    sfp_11_22 t; // how far along the hit occured
    sfp_11_22 u;
    sfp_11_22 v;
    uint_2 mat;
    vec3<int_11> color;

};

// WANT THIS TO BE COMBINATIOANL SO NO AC CHANNEL NOT SURE IF INPUT SHOULD BE CONST OR NOT ONCE AGIAN IM IFFY
// MAYBE MAKE THIS JUST A FUNCTION LIKE CCORE LIKE BRIAN BEEN DOIN BUT LEAVE THIS TO HIM
class HitRecordPhysics {
    
    public:

    HitRecordPhysics () {}

        void CCS_BLOCK(setFaceNormal)(hitRecord& record, 
                                        const Ray& r, 
                                        const full_vec3& outwardNormal) {

            record.front_face = dot(r.direction(), outwardNormal) < 0;
            record.normal = record.front_face ? outwardNormal : -outwardNormal;
        }
};
