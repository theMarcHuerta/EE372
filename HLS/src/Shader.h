#include <ac_int.h>
#include <ac_fixed.h>
#include <ac_channel.h>
#include <sstream>

#include "RTcore.h"

// Include mc_scverify.h for CCS_* macros
#include <mc_scverify.h>

class Shader
{
public:
    Shader(){}
    
#pragma hls_design interface
    void run(

            )
    {   
        // essentially implements equivalent C++ functionality and code
        // we have a class/ function for world hit and need to make one for each materials scatter function
        // MIGHT consider how much duplication of logic we need and maybe share some of the scatter area across cores?
    
        /*
            hit_record rec;

            // Check if the ray hits the background
            if (!world.hit(current_ray, interval(0.001, infinity), rec)) {
                accumulated_color += current_attenuation * background; // Apply background color
                break;
            }

            // After the first iteration, any ray is considered a secondary ray
            rec.is_secondary_ray = depth > 0;

            // Emitted light from the material (if any)
            color emitted = rec.mat->emitted(rec.u, rec.v, rec.p);
            accumulated_color += current_attenuation * emitted; // Add emitted light to accumulated color

            ray scattered; // The ray that results from scattering
            color attenuation; // The attenuation of the ray after scattering

            // If the material scatters the ray, update the current attenuation and the current ray
            if (rec.mat->scatter(current_ray, rec, attenuation, scattered)) {
                current_attenuation = current_attenuation * attenuation; // Update attenuation
                current_ray = scattered; // The scattered ray becomes the current ray for the next iteration
            } else {
                // If the material does not scatter the ray, no further color contributions are expected
                break;
            }
        }*/
    }
private:

};

#endif



