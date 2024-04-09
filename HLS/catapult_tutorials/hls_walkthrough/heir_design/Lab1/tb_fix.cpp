//
// Copyright 2003-2015 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
// 

#include "avg2.h"
#include "mc_scverify.h"

CCS_MAIN(int argv, char *argc){
  ac_channel<uint4 > din;
  ac_channel<uint4 > dout;

  for(int i=0;i<10;i++){
    uint4 tmp = i +1;
    //Added ac_int member function to convert to unsigned for printing
    printf("Testbench writing data%d = %d into channel\n\n",i, tmp.to_uint());
    din.write(tmp);//Write one value into the channel
    CCS_DESIGN(avg2)(din,dout);//Call top-level design
  }

  printf("\n");
  int cnt = 0;
  while(dout.available(1)){//While data is in the channel, print data
    printf("Testbench dout%d = %d\n", cnt, dout.read().to_int());
    cnt++;
  }
  CCS_RETURN(0);
}
