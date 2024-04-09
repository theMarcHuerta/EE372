//******************************************************************************
// File: sw_conv.h
//
// Description:
//   Pure software implementation of convolution and max pool
//******************************************************************************

#ifndef _INCLUDED_SW_CONV_H_
#define _INCLUDED_SW_CONV_H_

void conv (double *in, double *kern, double *out, int in_row, int in_col, int K_SIZE, int stride)
{
  double temp;
  unsigned int w_cnt,h_cnt;
  for (int i=0, w_cnt=0; i<in_row; i+= stride,w_cnt++) {
    if (i+K_SIZE-1 == in_row) { break; }
    for (int j=0,h_cnt=0; j<in_col; j+= stride,h_cnt++) {
      temp = 0;
      if (j+K_SIZE-1 == in_col) { break; }
      for (int k=0; k<K_SIZE; k++) {
        for (int l=0; l<K_SIZE; l++) {
          temp += *( in + (((i+k)*in_col)+(j+l)) ) * *( kern + (k*K_SIZE+l) ); //temp += in[i+k][j+l]*ker[k][l]
        }
      }
      *( out + w_cnt*((in_col-K_SIZE)/stride+1) + h_cnt ) = temp;
    }
  }
}

void max_pool(double *in, double *out, int in_row, int in_col, int K_SIZE)
{
  double temp;
  for (int i=0; i<in_row; i++) {
    if (i+K_SIZE-1 == in_row)
    { break; }
    for (int j=0; j<in_col; j++) {
      temp = 0;
      if (j+K_SIZE-1 == in_col) { break; }
      for (int k=0; k<K_SIZE; k++) {
        for (int l=0; l<K_SIZE; l++) {
          double inp_tmp = *( in + (((i+k)*in_col)+(j+l)) );
          temp =  (temp > inp_tmp) ? temp : inp_tmp; //temp > in[i+k][j+l] or what
        }
      }
      if (temp>0) {
        *( out + i*(in_col-K_SIZE+1) + j ) = temp;
      } else { //out[i][j] = temp
        * ( out + i*(in_col-K_SIZE+1) + j ) =  0;
      }
    }
  }
}

#endif

