//******************************************************************************
// File: eyeriss.h
//
// Description:
//
// Single Input channel Convolution CCORE
// Single Output feature map Convolution CCORE
// Reconfigurable width and height upto 1024
// Kernel Size is fixed
//
//******************************************************************************

#ifndef _INCLUDED_EYERISS_H_
#define _INCLUDED_EYERISS_H_

#include "types.h"
#include <mc_scverify.h>

//==============================================================================
// Class: lineBuffer
// Description:
//   This class implements a line buffer.
//
//==============================================================================

template <class elemType, class cfgType, int WIDTH, int KSIZE>
class lineBuffer
{
public:
  // Constructor
  lineBuffer() {
#pragma hls_unroll yes
    for (int i=0;i<KSIZE;i++)
      inp_reg[i] = 0;
  }

#pragma hls_design interface ccore
#pragma hls_pipeline_init_interval 1
  void CCS_BLOCK(run)(elemType &d_in, const bool rst_in, const bool weightEn, elemType d_out[KSIZE], cfgType width) {
    if (rst_in == true | weightEn == true) {
      done_r[0] = true;
      cnt = 0;
      w_cnt = width;
#pragma hls_unroll yes
      for (unsigned char i=0; i<KSIZE-1; i++) {
        done_r[i+1] = false;
        h_cnt[i] = 0;
      }
#pragma hls_unroll yes
      for (int i=0; i<KSIZE; i++) {
        d_out[i] = 0;
      }
    } else {
      inp_reg[0] = d_in;

      d_out[0] = inp_reg[0];

      temp_cnt = cnt;

#pragma hls_unroll yes
      for (unsigned char i=0; i<KSIZE-1; i++) {
        inp_reg[i+1] = line_buff[i][temp_cnt];
      }

#pragma hls_unroll yes
      for (unsigned char i=0; i<KSIZE-1; i++) {
        if (h_cnt[i] == i+1) {
          done_r[i+1] = true;
        }

        if (done_r[i+1]) {
          d_out[i+1] = inp_reg[i+1];
        } else {
          d_out[i+1] = 0;
        }

        if (done_r[i]) {
          line_buff[i][temp_cnt] = inp_reg[i];
        }
      }
      cnt++;
      if (cnt == w_cnt) {
        cnt = 0;
#pragma hls_unroll yes
        for (int i=0; i<KSIZE-1; i++) {
          h_cnt[i]++;
        }
      }
    }
  }

private:
  elemType     line_buff[KSIZE-1][WIDTH];                        // line buffer as blocks of RAM
  elemType     inp_reg[KSIZE];

  ac_int<ac::log2_ceil<WIDTH>::val + 1, false>   cnt;            // width counter
  ac_int<ac::log2_ceil<WIDTH>::val + 1, false>   h_cnt[KSIZE-1]; // height counter
  ac_int<ac::log2_ceil<WIDTH>::val, false>       temp_cnt;

  bool done_r[KSIZE];                                            // Done signal for individual blocks

  cfgType w_cnt;
};

//==============================================================================
// Class: cnnPE
// Description:
//    This class implements a Processing Element for the Eyeriss PE array.
//==============================================================================

template <class elemType, int KSIZE, int IFMAP, int BSize, int RSize, int YBits>
class cnnPE
{
  typedef typename elemType:: template rt_T<elemType>::mult T;
  typedef typename T::rt_unary:: template set<KSIZE+1>::sum resultType;

  typedef typename resultType::rt_unary:: template set<2>::sum tmpType;

public:
  cnnPE() { 
    outregWrite = 0;
    outregRead = 0;
    canRead = false;
#pragma hls_unroll yes
    for (int i=0;i<BSize;i++) {
      PixelWindow[i] = 0;
      WeightsRegs[i] = 0;
    }
  }

#pragma hls_design interface ccore
#pragma hls_pipeline_init_interval 1
  void CCS_BLOCK(run)(elemType &dataIn,
                      elemType &psumIn,
                      elemType &weightIn,
                      ac_int<YBits,false> idY, // "Y" position of element in array
                      bool &weightEn, // when true, shift weights into WeightsRegs
                      elemType &dataOut,
                      elemType &weightOut,
                      elemType &psumOut
                     ) 
  {
    // pass thru feature data
    dataOut = PixelWindow[0];
    weightOut = WeightsRegs[0];
    // Shift feature data in
#pragma hls_unroll yes
    PIXWINDOW: for (int i=0; i<KSIZE-1; i++) {
      PixelWindow[i] = PixelWindow[i+1];
    }
    PixelWindow[KSIZE-1] = dataIn;
    tmpType result = psumIn;
    // pass thru weights through if enabled
    if (weightEn) {
      macResult = 0;
#pragma hls_unroll yes
      WEIGHTWDW: for (int i=0; i<KSIZE-1; i++) {
        WeightsRegs[i] = WeightsRegs[i+1];
      }
      WeightsRegs[KSIZE-1] = weightIn;
    } else {
      macResult = PixelWindow[0] * WeightsRegs[0];
#pragma hls_unroll yes
      MAC: for (int i=1; i<KSIZE; i++) {
        macResult += PixelWindow[i] * WeightsRegs[i];
      }
    }
    // Circular buffer for the output data sync across the PEs
    OutputShiftReg[outregWrite] = macResult;
    outregWrite++;
    if(idY == 0) {
        result += macResult;
    } else {
      if(canRead) {
        result += OutputShiftReg[outregRead];
        outregRead++;
      } else {
        outregRead = 0;
      }
    }

    if(outregWrite == (idY*6)) {
      canRead = true;
    }

    // Compute partial sum output (taps output circular buffer)
    psumOut = result;
  }

private:
  elemType PixelWindow[BSize];     // feature data shifts through window
  elemType WeightsRegs[BSize];     // weights loaded during config
  elemType OutputShiftReg[RSize];  // output shift register
  resultType macResult;

  ac_int<YBits, false> outregRead;
  ac_int<YBits, false> outregWrite;
  bool canRead;
};

//==============================================================================
// Class: vld_CNN
// Description:
//        This class implements the valid signals for the PE outputs and it also implements the striding feature.
//        Striding reduces the feature map size by the striding factor * striding factor by processing the alternate pixel in a row and the row itself.
//==============================================================================

template <class configType, int KSIZE, int IFMAP, int OFMAP, int BSize, int RSize>
class vld_CNN
{
public:
  vld_CNN() {}

#pragma hls_design interface ccore
#pragma hls_pipeline_init_interval 1
  void CCS_BLOCK(run)(ac_int<ac::log2_ceil<OFMAP+1>::val, false> idX, configType stride, configType width, configType height, configType wdKsize, const bool &rst_in, bool &weightEn, bool &outVld) {
    if (rst_in | weightEn) {
      widthCntr = heightCntr = totalCnt = vldCnt = 0;
      totalCnt = wdKsize + ((KSIZE*IFMAP - 1)*6);
      vldCnt = wdKsize - width + KSIZE - 1 + ((KSIZE*IFMAP - 1)*6);
      outVld = false;
      widthVld = true;
      heightVld = true;
      oldwidthCntr = 0;
      oldheightCntr = 0;
      startCnt = idCnt = 0;
      start = (idCnt == idX);     // first column (first output channel) starts immediately

    } else {
      if (!start) {
        startCnt++;
        if (startCnt == KSIZE+1) {
          idCnt++;
          startCnt=0;
        }
        start = (idCnt == idX);
        outVld = false;
      } else {
        if (widthCntr == totalCnt) {
          totalCnt = width;
          widthCntr = 0;
          widthVld = true;
          if (heightVld) { oldheightCntr = heightCntr; }
          heightCntr++;
          if (oldheightCntr+stride == heightCntr) {
            heightVld = true;
          } else {
            heightVld = false;
          }
          vldCnt = KSIZE - 1;
        }
        outvldBuff = (widthCntr >= vldCnt) & (heightCntr < height-KSIZE+1);
        outVld = outvldBuff & widthVld & heightVld;
        widthCntr++;

        if (outvldBuff) {
          if (widthVld) {
            oldwidthCntr = strCntr;
          }
          strCntr++;
          if (oldwidthCntr+stride == strCntr) {
            widthVld = true;
          } else {
            widthVld = false;
          }
        } else {
          strCntr = 0;
        }
      }
    }
  }

private:
  configType widthCntr;             // Width Counter
  configType heightCntr;            // Height Counter
  configType oldwidthCntr;          // Stores the previuos width count - only in striding
  configType oldheightCntr;         // Stores the previuos height count - only in striding
  configType totalCnt;              // Current total loop count
  configType vldCnt;                // Current valid count
  configType strCntr;               // Stride counter

  configType startCnt;              // Start counter - only in multiple output channel
  configType idCnt;                 // id counter to validate the current vld_CNN block - only in multiple output channel

  bool outvldBuff;                  // Valid signal for the current output pixel
  bool widthVld;                    // Valid signal for the current width - only if striding is more than 1
  bool heightVld;                   // Valid signal for the current height - only if striding is more than 1
  bool start;                       // Start signal to start the current vld_CNN block
};

//==============================================================================
// Class: peArray
// Description:
//      This class implements the interconnection between the Processing elements, lineBuffers and the Valid checking modules.
//
//==============================================================================

template <class pType, class pvType, class configType, int WIDTH, int HEIGHT, int KSIZE, int IFMAP, int OFMAP>
class peArray
{
public: // helper enums
  enum {
    BSize = (1 << ac::log2_ceil<KSIZE>::val),       // power-of-2 size of pixel and weight buffer
    RSize = (1 << ac::log2_ceil<(KSIZE*IFMAP-1)*6 + 1>::val), // power-of-2 size of output regs buffer
    YBits = (ac::log2_ceil<(KSIZE*IFMAP-1)*6 + 1>::val)       // number of bits to represent a row number
  };

public:
  peArray() {
#pragma hls_unroll yes
    for(int i=0;i<5;i++) {
#pragma hls_unroll yes
      for(int j=0;j<IFMAP*KSIZE*OFMAP;j++) {
        pipelinePSUM[j][i] = 0;
      }
    }
#pragma hls_unroll yes
    for(int i=0;i<IFMAP*KSIZE;i++) {
#pragma hls_unroll yes
      for(int j=0;j<OFMAP;j++) {
        psumOut[j][i] = 0;
        dataOut[j][i] = 0;
      }
    }

  }

#pragma hls_design interface ccore
#pragma hls_pipeline_init_interval 1
  void CCS_BLOCK(run)(pType dataIn[IFMAP], 
                      pType partialIn[OFMAP], 
                      pType weightIn[IFMAP*KSIZE], 
                      configType stride, configType width, configType height, configType wdKsize, 
                      bool rst_in, bool weightEn, 
                      pvType outCNN[OFMAP]) 
  {
    {
#pragma hls_unroll yes
      for (int i=0; i<OFMAP; i++) {
        vldObj[i].run(i,stride, width, height, wdKsize, rst_in, weightEn, outVld[i]);
      }
    }
#pragma hls_unroll yes
    BUFFARR: for (int i=0; i<IFMAP; i++) {
      lineBuffers[i].run(dataIn[i], rst_in, weightEn, inBuff[i], width);
    }
#pragma hls_unroll yes
    for (int i=0; i<OFMAP; i++) {
#pragma hls_unroll yes
      OUTSHIFT: for (int k=(IFMAP*KSIZE)-1; k>0; k--) {
        psumIn[i][k] = pipelinePSUM[i*IFMAP*KSIZE + k][4];
#pragma hls_unroll yes
        for(int j=4;j>0;j--){
          pipelinePSUM[i*IFMAP*KSIZE + k][j] = pipelinePSUM[i*IFMAP*KSIZE + k][j-1];
        }
        pipelinePSUM[i*IFMAP*KSIZE + k][0] = psumOut[i][k-1];
      }
      psumIn[i][0] = partialIn[i];
    }
#pragma hls_unroll yes
    for (int k=0; k<(IFMAP*KSIZE); k++) {
#pragma hls_unroll yes
      for (int i=OFMAP-1; i>0; i--) {
        buffIN[i][k] = dataOut[i-1][k];
        buffWeight[i][k] = weightOut[i-1][k];
      }
      buffIN[0][k] = inBuff[k/KSIZE][k%KSIZE];
      buffWeight[0][k] = weightIn[k];
    }
#pragma hls_unroll yes
    for (int i=OFMAP-1; i>0; i--) {
      weightEnbuff[i] = weightEnbuff[i-1];
    }
    weightEnbuff[0] = weightEn;

#pragma hls_unroll yes
    OFMAP: for (int x=0; x<OFMAP; x++) {
#pragma hls_unroll yes
      ARRAY_INTERCONNECT: for (int y=0; y<KSIZE*IFMAP; y++) {
        // (x,y) represents the "position" of this element in the array
        peInstArray[x][y].run(buffIN[x][y], psumIn[x][y], buffWeight[x][y], y, weightEnbuff[x], dataOut[x][y], weightOut[x][y], psumOut[x][y]);
      }
    }

#pragma hls_unroll yes
    OUTPUT: for (int i=0; i<OFMAP; i++) {
      outCNN[i].data = psumOut[i][(IFMAP*KSIZE)-1];
      outCNN[i].vld = outVld[i];
    }
  }
private:
  cnnPE<pType,KSIZE,IFMAP,BSize,RSize,YBits>          peInstArray[OFMAP][KSIZE*IFMAP];      // PE array
  lineBuffer<pType,configType,WIDTH,KSIZE>            lineBuffers[IFMAP];                   // Line buffer array
  vld_CNN<configType,KSIZE,IFMAP,OFMAP,BSize,RSize>   vldObj[OFMAP];                        // Valid checking block array

  //Interconnections
  pType psumOut[OFMAP][KSIZE*IFMAP];
  pType psumIn[OFMAP][KSIZE*IFMAP];
  pType buffWeight[OFMAP][KSIZE*IFMAP];
  pType buffIN[OFMAP][KSIZE*IFMAP];
  pType dataOut[OFMAP][KSIZE*IFMAP];
  pType weightOut[OFMAP][KSIZE*IFMAP];
  pType inBuff[IFMAP][KSIZE];
  pType pipelinePSUM[IFMAP*KSIZE*OFMAP][5];

  bool outVld[OFMAP];
  bool weightEnbuff[OFMAP];
};

//==============================================================================
// Class: imageProcessor
// Description:
//      This class controls the operations of the PE Array such. It includes loading of the configuration data, weights from the memory and streaming in and out the feature maps.
//
//==============================================================================

#pragma hls_design top
template <class pType, class pvType, class configType, int WIDTH, int HEIGHT, int KSIZE, int IFMAP, int OFMAP, int PADDING>
class imageProcessor
{
public: // helper enums
  enum {
    WR = eyerissHelper<WIDTH,HEIGHT,KSIZE,IFMAP,OFMAP>::weightrowVal,
    WC = eyerissHelper<WIDTH,HEIGHT,KSIZE,IFMAP,OFMAP>::weightcolVal,
    numWeights = OFMAP * KSIZE
  };

public:
  imageProcessor() {
    mainIter = startIter = 0;
#pragma hls_unroll yes
    for (int i=0; i<OFMAP; i++) {
      psumIn[i] = 0;
    }
    w_cnt = h_cnt = 0;
  }

#pragma hls_design interface
#pragma hls_pipeline_init_interval 1
  void CCS_BLOCK(run)(ac_channel<packedFData<pType,IFMAP> > &inCNN, ac_channel<ac_int<62, false> > &configIn, ac_channel<packedFData<pvType,OFMAP> > &outCNN) {
    const pType weightIn[WC][WR*4] = {
       /*  Outline  |  sharpen  |      Gaussian Blur      |Embossing| */ 
      { -1, -1,  -1,  0, -1,  0, 0.0625,  0.125,  0.0625,  0,  1, 2, 0, 0, 0, 0 },
      { -1,  8,  -1, -1,  5, -1, 0.125 ,  0.25 ,  0.125 , -1,  1, 1, 0, 0, 0, 0 },
      { -1, -1,  -1,  0, -1,  0, 0.0625,  0.125,  0.0625, -2, -1, 0, 0, 0, 0, 0 },
      {  0,  0,   0,  0,  0,  0, 0,       0,      0,       0,  0, 0, 0, 0, 0, 0 }
    };
     configType config[6];
     ac_int<62, false> tempConfigIn = configIn.read();
    do {
      // Config Memory data
      //   0 - read 'Stride' configuration
      //   1 - read 'width' configuration
      //   2 - read 'height' configuration
      //   3 - read 'widthKsize' configuration
      //   4 - read 'imgsz' configuration
      //   5 - read ' filter select' configuration
      // mainIter represent the state of the design
      //   0 - reset
      //   1 - read 'weight[N]'
      //   ...
      //   1+N
      //   1+N+1 - read feature data

      LDCONFIG: if (mainIter == 0) {
      config[0] = tempConfigIn.slc<3>(0);   // Stride
      config[1] = tempConfigIn.slc<11>(3);  // Width
      config[2] = tempConfigIn.slc<11>(14); // Height
      config[3] = tempConfigIn.slc<13>(25); // Pad_width*KSIZE
      config[4] = tempConfigIn.slc<22>(38); // Pad_width * Pad_Height (Img size)
      config[5] = tempConfigIn.slc<2>(60);  // Filter Option
        padWidth = config[1] + 2*PADDING;
        padHeight = config[2] + 2*PADDING;
        loopCnt = config[4] + IFMAP*KSIZE*6 + OFMAP*KSIZE + OFMAP - 5;  // Total execution count once the input pixel sequence starts (Total image size + flushing count)
        reset = true;
        weightEn = false;
        weightCnt = 0;
      } else if (mainIter > 0 & mainIter < numWeights + 1) {
        // Weight Loading
        reset = false;
        weightEn = true;
#pragma hls_unroll yes
        for (int i=0; i<WC; i++) {
          if (i < IFMAP*KSIZE) {  // Since the column of the weight matrix is greater than IFMAP*KSIZE, we don't have to read the location after (IFMAP*KSIZE)
            ac_int<ac::log2_ceil<WC>::val,false> tmp_cnt = i;
            weight[i] = weightIn[tmp_cnt][((ac_int<2,false>)config[5])*KSIZE + weightCnt];
          }
        }
        weightCnt++;
      } else {
        reset = weightEn = false;
        if (startIter < config[4]) {
          // read input data
          packedFData<pType,IFMAP> input;
          if((w_cnt >= PADDING & w_cnt < config[1] + PADDING) & (h_cnt >= PADDING & h_cnt < config[2] + PADDING)) {
            input = inCNN.read(); // parallel read of IFMAP channels of the current pixel
          } else {
#pragma hls_unroll yes
            for(int i=0;i<IFMAP;i++)
              input.data[i] = 0;
          }
#pragma hls_unroll yes
          for (int i=0; i<IFMAP; i++) {
            dataIn[i] = input.data[i];
          }
        }
        startIter++;
        w_cnt++;
        if (w_cnt == padWidth) {
          w_cnt = 0;
          h_cnt++;
        }
      }
      arrayInst.run(dataIn, psumIn, weight, config[0], padWidth, padHeight, config[3], reset, weightEn, psumOut);
#pragma hls_unroll yes
      for (int i=0; i<OFMAP; i++) {
        output.data[i].data = psumOut[i].data;
        output.data[i].vld = psumOut[i].vld;
      }
      outCNN.write(output);
      mainIter++;
      done = (startIter == loopCnt);
    } while (!done);

  }

private:
  peArray<pType,pvType,configType,WIDTH,HEIGHT,KSIZE,IFMAP, OFMAP> arrayInst;
  pType weight[IFMAP*KSIZE];
  pType dataIn[IFMAP];
  pType psumIn[OFMAP];
  pvType psumOut[OFMAP];
  packedFData<pvType,OFMAP> output;
  ac_int< eyerissHelper<WIDTH,HEIGHT,KSIZE,IFMAP,OFMAP>::cntwidthVal, false> mainIter;
  ac_int< eyerissHelper<WIDTH,HEIGHT,KSIZE,IFMAP,OFMAP>::cntwidthVal, false> loopCnt;
  configType startIter;
  ac_int<ac::log2_ceil<WR>::val,false> weightCnt;

  configType padWidth;
  configType padHeight;
  configType h_cnt;
  configType w_cnt;

  bool reset;
  bool weightEn;
  bool done;
};

#endif

