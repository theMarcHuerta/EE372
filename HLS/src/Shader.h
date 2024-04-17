#include <ac_int.h>
#include <ac_fixed.h>
#include <ac_channel.h>
#include <sstream>

#include "RTcore.h"
#include "HitRecord.h"

// Include mc_scverify.h for CCS_* macros
#include <mc_scverify.h>

class Shader
{
public:
    Shader(){}
    
#pragma hls_design interface
    void CCS_BLOCK(run)(ac_channel<img_params> params_in,
                        ac_channel<quad_hittable> &quad_in,
                        ac_channel<quad_hittable> &sphere_in,
                        ac_channel<ray> &ray_in,
                        ac_channel<rgb_t<sfp_9_10>> attenuation_chan_in,
                        ac_channel<rgb_t<sfp_9_10>> accumalated_color_chan_in,
                        ac_channel<rgb_t<sfp_9_10>> attenuation_chan_out,
                        ac_channel<rgb_t<sfp_9_10>> accumalated_color_out,
                        ac_channel<ray> &ray_out,
            )
    {   

        HitRecord rec;
        img_params params = params_in.read();
        bool isHit;
        worldHit.hit(ray_in, params.num_spheres, params.num_quads, sphere_in, quad_in, worldRec, isWorldHit):
        rec = worldRec.read();
        isHit = isWorldHit.read();
        sfp_9_10 color_in;
        color_in = accumalated_color_chan_in.read();
        sfp_9_10 atten_chan;
        atten_in = attenuation_chan_in.read();
        if (!isHit) {
            accumulated_color += atten_in * params.background; // HAVE TO MAKE SURE I CHOP THIS INTO THE RIGHT FORMAT HERE, TURN BACKGROUND HERE INTO LONGER FORMAT FP
        }

        //add secondary ray logic in later
        // // After the first iteration, any ray is considered a secondary ray
        // rec.is_secondary_ray = depth > 0;

        sfp_9_10 tmp_color_out;
        sfp_9_10 tmp_atten_out;
        tmp_atten_out = attenuation_chan_in.read();
        tmp_color_out = accumalated_color_chan_in.read();
        if (rec.mat == 0){ // lambertian
            ray scattered_ray;
            bool doesScatter;
            lambertianScatter.scatter();
            ray_out.write(scattered_ray);
            if (doesScatter){
                tmp_color_out += obj.color * tmp_atten_out ; // EDIT WORLD HIT TO JUST SEND OUT OBJ COLOR
                accumalated_color_out.write(tmp_color_out);
                attenuation_chan_out.write(tmp_atten_out);
            }
            else {
                sfp_9_10 max_atten(1,1,1); // MAKE CONSTRUCTOR
                attenuation_chan_out.write(max_atten);
            }
        }
        else if (rec.mat == 1){ // metal
            ray scattered_ray;
            bool doesScatter;
            metalScatter.scatter();
            ray_out.write(scattered_ray);
            if (doesScatter){

            }
            else {
                sfp_9_10 max_atten(1,1,1); // MAKE CONSTRUCTOR
                attenuation_chan_out.write(max_atten);
            }
        }
        else if (rec.mat == 2){ //emission
            tmp_color_out += obj.color * tmp_atten_out ; // EDIT WORLD HIT TO JUST SEND OUT OBJ COLOR
            accumalated_color_out.write(tmp_color_out);
            attenuation_chan_out.write(tmp_atten_out);
        }
        
    }
private:
    WorldHit worldHit;
    ac_channel<bool> &isWorldHit;
    ac_channel<HitRecord> worldRec;
    LambScatter lambertianScatter;
    MetalScatter metalScatter;
};

#endif



