#pragma once
#include <ac_channel.h>
#include <ac_int.h>
#include <mc_scverify.h>

struct pack{
  ac_int<16> data[4];
};

class simple{
  ac_int<34> acc;
  public:
  simple(){}
  #pragma hls_design interface
  void CCS_BLOCK(run)(ac_channel<pack> &d0_in, ac_channel<pack> &d1_in, 
    ac_int<34> bias[32], ac_int<5,false> addr, ac_channel<ac_int<34>> &dout){
    acc = 0;
    //read the input streaming channels
    pack d0 = d0_in.read();
    pack d1 = d1_in.read();
    MAC: for(int i=0; i<4; i++){
      acc += d0.data[i] * d1.data[i];
    }
    dout.write(acc + bias[addr]);
  }
};

