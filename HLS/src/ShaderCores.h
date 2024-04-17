#include <ac_int.h>
#include <ac_fixed.h>
#include <ac_channel.h>
#include <sstream>

#include "RTcore.h"

// Include mc_scverify.h for CCS_* macros
#include <mc_scverify.h>

class ShaderCoresLoop
{
public:
    ShaderCoresLoop(){}
    
#pragma hls_design interface
    void run(

            )
    {
      // will take in all the params from the wrapper and then we have to make 
      //an accumaltion buffer of size 8 (if that how many bounce depth we go with)
      // we'll have a pipeline of 8 shader cores and the output of the previous will go to the next
      // in each shader, we check world hit, emmission, then accumalte color based of the materials scatter function
      // this loop function will just set up the pipeline correclty and variables that will pass from shader to shader 
      // and at the end we'll just output accumalted color
    }
private:
  Shader shader1;
  Shader shader2;
  Shader shader3;
  Shader shader4;
  Shader shader5;
  Shader shader6;
  Shader shader7;
  Shader shader8;
};

#endif


class ShaderCoresWrapper
{
public:
    ShaderCoresWrapper(){}
    
#pragma hls_design interface
    void run(

            )
    {
      // pass in all variables to loop 
      // should be the ray and access to the buffer/ram holding the sphere and quad hittables/primitaives
      // this should return a stream of outputted pixels / vec3 of 8 bits
    }
private:
};

