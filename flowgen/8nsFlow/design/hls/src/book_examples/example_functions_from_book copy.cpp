#include "ac_int.h"
#include "ac_channel.h"
#define num_taps 8
typedef ac_channel< ac_int<18> > Channel;
#pragma design
template<int ID>// WHAT IS TEMPLATE
void fir_filter ( Channel &input, ac_int<18> coeffs[num_taps], Channel &output ) {
    #ifndef __SYNTHESIS__
    while ( input.available(1) )
    #endif
    {
        static ac_int<18> regs[num_taps];
        int i, temp = 0;
        SHIFT:for ( i = num_taps-1; i>=0; i--) {
            if ( i == 0 )
                regs[i] = input.read();
            else
                regs[i] = regs[i-1];
            }
        MAC:for ( i = num_taps-1; i>=0; i--) {
            temp += coeffs[i]*regs[i];
        }
    output.write(temp>>18);
    }
}
#pragma design top
void fir_cascade ( Channel &input, ac_int<18> coeffs0[num_taps], ac_int<18>
coeffs1[num_taps], Channel &output ) {
    static Channel intermediate;
    fir_filter<0>(input,coeffs0,intermediate);
    fir_filter<1>(intermediate,coeffs1,output);
}



//////////////////////////////////////////////////////////////////////////////////



#include "ac_int.h"
#include "ac_channel.h"

#pragma design
template<int ID>
void block( ac_channel< ac_int<18> > &i, ac_int<18> c,
ac_channel< ac_int<18> > &o ) {
#ifndef __SYNTHESIS__
    while ( i.available(1) )
#endif
    {
        o.write( i.read() * c );
    }
}

#pragma design top
void ac_chan_basic ( ac_channel< ac_int<18> > &i, ac_int<18> c,
ac_channel< ac_int<18> > &o ) {
    static ac_channel< ac_int<18> > intermediate;
    block<0>(i,c,intermediate);
    block<1>(intermediate,c,o);
}





//////////////////////////////////////////////////////////////////////////////////



void ac_chan_basic ( ac_channel< ac_int<18> > &i, ac_int<18> c,
ac_channel< ac_int<18> > &o ) {
    static ac_channel< ac_int<18> > intermediate1;
    static ac_channel< ac_int<18> > intermediate2;
    static ac_channel< ac_int<18> > intermediate3;
    static ac_channel< ac_int<18> > intermediate4;
    static ac_channel< ac_int<18> > intermediate5;
    static ac_channel< ac_int<18> > intermediate6;
    static ac_channel< ac_int<18> > intermediate7;
    static ac_channel< ac_int<18> > intermediate8;



    shader<0>(i,c,intermediate);
    shader<1>(intermediate,c,o);
    shader<2>(intermediate,c,o);
    shader<3>(intermediate,c,o);
    shader<4>(intermediate,c,o);
    shader<5>(intermediate,c,o);
    shader<6>(intermediate,c,o);
    shader<7>(intermediate,c,o);
}





//////////////////////////////////////////////////////////////////////////////////



void function_name (ac_channel<T> &in1, ac_channel<oT1> &out1, ...) {
    #ifndef __SYNTHESIS__
    while ( in1.available(num_needed) && in2.available(num_needed) && ... )
    #endif
    {
        <design body>
    }
}

/*
The while-loop condition must make sure to allow the while-loop to run only when exactly the right number
of inputs are available. If the number of inputs is variable, then the [ ] operator can be used to inspect
elements in the channel for the simulation code. For example, the following code has a num_samples input
that can be used to control the number of samples read. In this case, the first element in num samples is
inspected and used to make sure that there are enough samples to run the code:*/
#include <ac_channel.h>
void my_design ( ac_channel<int> &input1, ac_channel<int> &input2,
ac_channel<int> &output) {
    #ifndef __SYNTHESIS__
    while ( input.available(2) && input2.available(1))
    #endif
    {
        int temp = input1.read()*5 + input2.read()*7;
        temp += input1.read()*7;
        output->write(temp);
    }
}

#ifndef __SYNTHESIS__
while ( num_samples.available(1) && samples.available(num_samples[0]) )
#endif
    {
    tmp_num_samples = num_samples.read();
    for ( int i = 0; i < tmp_num_samples; i++ ) {
        f(samples.read());
    }
}


#ifndef __SYNTHESIS
while ( in.available(1) || intermediate.available(5) )
#endif
{
    static ac_channel<T> intermediate;
    function1(in,intermediate);
    function2(intermediate,out);
}



////////////////////////////////////////////////////////////////////////


#ifndef _FIR__H
 #define _FIR__H

 #include <ac_int.h>
 #include <ac_channel.h>
 #include <mc_scverify.h>

 class fir
 {
 ac_int<8> regs[8];

 public:

    fir() // Required constructor
    {
    for (int i = 7; i>=0; i--)
        { regs[i] = 0; } // Initialization of taps
    }

    #pragma hls_design interface
    void CCS_BLOCK(run)(ac_channel< ac_int<8> > &input,
    ac_int<8> coeffs[8],
    ac_channel< ac_int<8> > &output) {
        ac_int<19> temp = 0;

        SHIFT:for (int i = 7; i>=0; i--) {
            if ( i == 0 )
                { regs[i] = input.read(); }
            else
                { regs[i] = regs[i-1]; }
            }
        MAC:for (int i = 7; i>=0; i--) {
            temp += coeffs[i]*regs[i];
        }
        output.write(temp>>11);
    }
};
#endif


/////////////////////////////////////////////////////////////////////////////////////


#ifndef _FIR_TEMPLATE_H
 #define _FIR_TEMPLATE_H

 #include <ac_int.h>
 #include <ac_channel.h>
 #include <mc_scverify.h>

 template <class inType, class coeffType, class outType, class accType, int num_taps>
 class fir_template
 {
 coeffType regs[num_taps];

 public:
    fir_template() // Required constructor
    {
    for (int i = num_taps - 1; i>=0; i--)
        { regs[i] = 0; } // Initialization of taps
    }

    #pragma hls_design interface
    void CCS_BLOCK(run)(ac_channel<inType> &input,
    coeffType coeffs[num_taps],
    ac_channel<outType> &output) {
    accType temp = 0;

        SHIFT:for (int i = num_taps - 1; i>=0; i--) {
            if ( i == 0 )
                { regs[i] = input.read(); }
            else
                { regs[i] = regs[i-1]; }
            }
        MAC:for (int i = num_taps - 1; i>=0; i--) {
            temp += coeffs[i]*regs[i];
        }
        output.write(temp>>11);
    }
 };
 #endif


//////////////////////////////////////////////////////////////////////////////////


#include "fir.h"
 #include "decimator.h"

 class top {

    ac_channel<ac_int<8> > connect0;
    ac_channel<ac_int<8> > connect1;

    fir block0;
    fir block1;
    decimator block2;

    public :

    top () {}

    #pragma hls_design interface top
    void CCS_BLOCK(run) (ac_channel<ac_int<8> > &din,
                        ac_int<8> coeffs0[8],
                        ac_int<8> coeffs1[8],
                        ac_channel<ac_int<8> > &dout) {

        block0.run(din,coeffs0,connect0);
        block1.run(connect0,coeffs1,connect1);
        block2.run(connect1,dout);
    }
 };



  #include "fir.h"

 class top_subprocess_block {

    #pragma hls_design
    void decimator (ac_channel<ac_int<8> > &din,
                    ac_channel<ac_int<8> > &dout) {
        if (count==4)
            {count = 0;}
        ac_int<8> temp = din.read();
        if (count==0)
            {dout.write(temp);}
        count++;
    }

    int count;
    ac_channel<ac_int<8> > connect0;
    ac_channel<ac_int<8> > connect1;

    fir block0;
    fir block1;

    public :

    top_subprocess_block () {
        count = 0;
        }

        #pragma hls_design interface top
        void CCS_BLOCK(run) (ac_channel<ac_int<8> > &din,
                            ac_int<8> coeffs0[8],
                            ac_int<8> coeffs1[8],
                            ac_channel<ac_int<8> > &dout) {

            block0.run(din,coeffs0,connect0);
            block1.run(connect0,coeffs1,connect1);
            decimator(connect1,dout);
        }
 };



////////////////////////////////////////////////////////////////////////////////



template<typename T, int N>
struct chanStruct{
    T data[N];
};

#pragma hls_design
void BLOCK0(dType din[NUM_WORDS],
            ac_channel<chanStruct<dType,NUM_WORDS> > &dout){
    chanStruct<dType,NUM_WORDS> tmp; //temporary array inside struct
    WRITE:for(dType i=0;i<NUM_WORDS;i++){
        tmp.data[i] = din[i];
    }
    dout.write(tmp);//Memory channel write
}
#pragma hls_design
void BLOCK1(ac_channel<chanStruct<dType,NUM_WORDS> > &din,
            dType dout[NUM_WORDS]){
    chanStruct<dType,NUM_WORDS> tmp; //temporary array inside struct
    tmp = din.read(); // Single Memory channel read
    READ:for(dType i=NUM_WORDS-1;i>=0;i--){
    dout[i] = tmp.data[i];
    }
}
#pragma design top
void top(dType din[NUM_WORDS],dType dout[NUM_WORDS]){
    static ac_channel<chanStruct<dType,NUM_WORDS> > shr_mem;//Static memory channel
    BLOCK0(din,shr_mem);
    BLOCK1(shr_mem,dout);
}






#pragma hls_design
void BLOCK0(ac_channel<int> &din,
            ac_channel<shr_mem_struct> &dout // Memory Write Channel
    ){
    shr_mem_struct tmp; //temporary array inside struct
    WRITE:for(int i=0;i<64;i++){ //Local Operations
        tmp.data[i] = din.read();
    }
    dout.write(tmp); // Single Memory channel write
    //DO NOT access the local struct after this point
}

////////////BADDDD
void BLOCK0(ac_channel<int> &a,
            ac_channel<shr_mem_struct> &z,
            ac_int<8,false> bounds) {
    shr_mem_struct mem; // State is carried between outer loop iterations
    for (int i=0; i<4; i++) {
        for (int j=0; j<256; j++) {
            mem.dat[j] = a.read() ^ 0xDEADBEEF;
            if(j == bounds-1)
                break;
        }
    z.write(mem);
    }   
}

/// GOOD
void BLOCK0(ac_channel<int> &a,
            ac_channel<shr_mem_struct> &z,
            ac_int<8,false> bounds) {
    for (int i=0; i<4; i++) {
        shr_mem_struct mem; // local struct is reconstructed for each iteration
        for (int j=0; j<256; j++) {
            mem.dat[j] = a.read() ^ 0xDEADBEEF;
            if(j == bounds-1)
                break;
        }
        z.write(mem);   
    }
}

#pragma design top
void top(dType din[NUM_WORDS],dType dout[NUM_WORDS]){
//Static memory channel
    static ac_channel<chanStruct<dType,NUM_WORDS> > shr_mem;
    BLOCK0(din,shr_mem);
    BLOCK1(shr_mem,dout);
}


















































