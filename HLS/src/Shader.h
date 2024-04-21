#include <ac_int.h>
#include <ac_fixed.h>
#include <ac_channel.h>
#include <sstream>

#include "RTcore.h"
#include "HitRecord.h"
#include "WorldHit.h"
#include "MaterialScatter.h"
// Include mc_scverify.h for CCS_* macros
#include <mc_scverify.h>

class Shader
{
public:
    Shader(){}
    
#pragma hls_design interface
    void CCS_BLOCK(run)(ac_channel<img_params> &params_in,
                        ac_channel<quad_hittable> &quad_in,
                        ac_channel<sphere_hittable> &sphere_in,
                        ac_channel<ray<sfp_11_22>> &ray_in,
                        ac_channel<rgb_t<sfp_9_10>> &attenuation_chan_in,
                        ac_channel<rgb_t<sfp_11_10>> &accumalated_color_chan_in,
                        ac_channel<rgb_t<sfp_9_10>> &attenuation_chan_out,
                        ac_channel<rgb_t<sfp_11_10>> &accumalated_color_out,
                        ac_channel<ray<sfp_11_22>> &ray_out
                        )
    {   

        worldHit.hit(ray_in, params_in, sphere_in, quad_in, attenuation_chan_in, accumalated_color_chan_in, 
                     attenuation_chan_through, accumalated_color_through, worldRec, rayToScatter, isWorldHit, paramsThrough);
        materialScatter.scatter(rayToScatter, paramsThrough, attenuation_chan_through, accumalated_color_through, isWorldHit,
                                 attenuation_chan_out, accumalated_color_out, ray_out);

    }
private:
    WorldHit worldHit;
    MaterialScatter materialScatter;

    ac_channel<ray<sfp_11_22>> rayToScatter;
    ac_channel<rgb_t<sfp_9_10>> attenuation_chan_through;
    ac_channel<rgb_t<sfp_11_10>> accumalated_color_through;
    ac_channel<bool> isWorldHit;
    ac_channel<HitRecord> worldRec;
    ac_channel<img_params> paramsThrough;
};

#endif



