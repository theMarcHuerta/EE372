#include <iostream>

#include "Renderer.cpp"

using namespace std;

CCS_MAIN(int argc, char** argv) {
    Renderer inst; // DUT
    ac_channel<pack> d0_in;
    ac_channel<pack> d1_in;
    ac_channel<ac_int<34> > dout;
    ac_int<34> bias[32];
    ac_int<5,false> addr = 2;
    pack d0, d1;
    
    for(int i=0; i<10; i++){
        bias[i] = i;
    }

    for(int i=0; i<10; i++){
        for (int j=0; j<4; j++){
            d0.data[j] = i;
            d1.data[j] = i;
        }
        d0_in.write(d0);
        d1_in.write(d1);
        inst.run(d0_in, d1_in, bias, addr, dout);
    }
    while(dout.available(1)){
         cout << dout.read().to_int() << endl;
    }

    CCS_RETURN(0);
}