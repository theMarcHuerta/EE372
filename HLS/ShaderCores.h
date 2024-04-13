#ifndef _GLOBAL_SIMPLE_H
#define _GLOBAL_SIMPLE_H

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

    }
private:

};

#endif


class ShaderCores
{
public:
    ShaderCores(){}
    
#pragma hls_design interface
    void run(

            )
    {

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