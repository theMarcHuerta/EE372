// ac_sincos_lut_tb
//
// Tests the ac_sincos_lut() function with the ac_fixed type

// Usage:
// g++ -I$MGC_HOME/shared/include -DTEST_SINCOS_AC_FIXED ac_sincos_lut_tb.cpp -o test_ac_sincos_lut
// ./test_ac_sincos_lut

#include "ac_sincos_lut_tb.h"

//
#include <ac_math/ac_sincos_lut.h>
using namespace ac_math;

#include <mc_scverify.h>

#pragma hls_design top
void CCS_BLOCK(project)(const input_type     &angle,
                        output_type          &output)
{
  ac_sincos_lut(angle, output);
}

// ========================= TESTBENCH ====================================================
#if !defined(__SYNTHESIS__) && !defined(__MATLAB_CATAPULT__)

#include <stdlib.h>
#include <cstdlib>
#include <math.h>
#include <cstring>

#include <fstream>
#include <iostream>
using namespace std;

#include <ac_dsp_utils.h>

#define PLOT_SINCOS_FILE_WRITE

CCS_MAIN(int argc, char *argv[])
{
#ifdef PLOT_SINCOS_FILE_WRITE
  //File to print values to be plotted in matlab
  const char filename[] = "plot_values.csv";
  ofstream outfile(filename);

  outfile << "Input(double), Input(FixPt), ExpectSin(double), ExpectCos(double), LUTSin(double), LUTSin(FixPt), LUTCos(double), LUTCos(FixPt), ErrorSin(percent), ErrorCos(percent) " << endl;
#endif

  cout << "==================== ac_sincos_lut Test ======================" << endl;

  //Declare inputs and outputs of all different types to be tested.
  input_type angle;
  output_type output;
  output_ri_type cos_output;
  output_ri_type sin_output;

  double lower_limit, upper_limit, step, allowed_error;

  //set ranges and step size for fixed point testbench

  if ( argc < 5 ) {
    cout << "Warning: using default testbench ranges" << endl;
    lower_limit   = 0;
    upper_limit   = 1;
    step          = (angle.template set_val<AC_VAL_QUANTUM>()).to_double();
    allowed_error = 1.0;
  } else {
    lower_limit   = atof(argv[1]);
    upper_limit   = atof(argv[2]);
    if (strcmp("-", argv[3]) != 0) {
      step          = atof(argv[3]);
    } else {
      step = angle.template set_val<AC_VAL_QUANTUM>().to_double();
    }
    allowed_error = atof(argv[4]);
  }
  cout << "lower_limit      = " << lower_limit << endl;
  cout << "upper_limit      = " << upper_limit << endl;
  cout << "step             = " << step << endl;
  cout << "allowed_error    = " << allowed_error << endl;
  cout << endl;

  ac_channel<double> expectSinVals;
  ac_channel<double> actualSinVals;
  ac_channel<double> expectCosVals;
  ac_channel<double> actualCosVals;
  std::vector<double> sampleVals;

  for (double i = lower_limit; i < upper_limit; i += step) {
    sampleVals.push_back(i);
    angle = input_type(i);
    CCS_DESIGN(project) (angle, output);

    double rad = angle.to_double()*2*M_PI;
    double expected_sin = sin(rad);
    double expected_cos = cos(rad);

    cos_output = output.r();
    sin_output = output.i();

    ac_channel<double> tmpExpSinVals;
    ac_channel<double> tmpActSinVals;
    ac_channel<double> tmpExpCosVals;
    ac_channel<double> tmpActCosVals;

    expectSinVals.write(expected_sin);
    expectCosVals.write(expected_cos);
    actualSinVals.write(sin_output.to_double());
    actualCosVals.write(cos_output.to_double());

    tmpExpCosVals.write(expected_cos);
    tmpExpSinVals.write(expected_sin);
    tmpActCosVals.write(cos_output.to_double());
    tmpActSinVals.write(sin_output.to_double());

    int tmpIdx;
    double thisSinErr;
    double thisCosErr;
    get_stats(tmpExpSinVals, tmpActSinVals, thisSinErr, tmpIdx, 0, .0001);
    get_stats(tmpExpCosVals, tmpActCosVals, thisCosErr, tmpIdx, 0, .0001);

#if 0
    cout << " The input angle is: " << angle << endl;
    cout << " The expected value of sin is: " << expectedsin << endl;
    cout << " The actual value of sin is: " << actualsin << endl;
    cout << " The expected value of cos is: " << expectedcos << endl;
    cout << " The actual value of cos is: " << actualcos << endl;
#endif

#ifdef PLOT_SINCOS_FILE_WRITE
  outfile << i << ",   " << angle << ",   " << expected_sin << ",   " << expected_cos << ",   " 
          << sin_output.to_double() << ",   " << sin_output << ",   " 
          << cos_output.to_double() << ",   " << cos_output << ",   " 
          << thisSinErr << ",   " << thisCosErr << std::endl;


#endif
  }
#ifdef PLOT_SINCOS_FILE_WRITE
  //Close file
  outfile.close();
#endif

  double max_sin_error = 0;
  double max_sin_error_sample = 0;
  double max_sin_error_actual = 0;
  double max_sin_error_expected = 0;
  int   maxSinIdx = -1;

  double max_cos_error = 0;
  double max_cos_error_sample = 0;
  double max_cos_error_actual = 0;
  double max_cos_error_expected = 0;
  int   maxCosIdx = -1;

  get_stats(expectSinVals, actualSinVals,  max_sin_error, maxSinIdx, 0, .0001);
  get_stats(expectCosVals, actualCosVals,  max_cos_error, maxCosIdx, 0, .0001);

  if (maxSinIdx != -1) {
    max_sin_error_sample =   sampleVals[maxSinIdx];
    max_sin_error_actual =   actualSinVals[maxSinIdx];
    max_sin_error_expected = expectSinVals[maxSinIdx];
  }
  if (maxCosIdx != -1) {
    max_cos_error_sample =   sampleVals[maxCosIdx];
    max_cos_error_actual =   actualCosVals[maxCosIdx];
    max_cos_error_expected = expectCosVals[maxCosIdx];
  }
  cout << "Testbench finished" << endl;
  cout << "Cosine: max_error =" << max_cos_error << "  with sample =" << max_cos_error_sample 
       << "  expected=" << max_cos_error_expected << "  actual=" << max_cos_error_actual << endl;

  cout << "Sine: max_error =" << max_sin_error << "  with sample =" << max_sin_error_sample 
       << "  expected=" << max_sin_error_expected << "  actual=" << max_sin_error_actual << endl;

  bool isErr = false;
  if (max_cos_error > allowed_error) {
    cout << "Cosine error tolerance of " << allowed_error << " percent error exceeded - FAIL" << endl;
    isErr = true;
  }
  if (max_sin_error > allowed_error) {
    cout << "Sine error tolerance of " << allowed_error << " percent error exceeded - FAIL" << endl;
    isErr = true;
  }
  cout << "=============================================================================" << endl;

  if (isErr) {
    CCS_RETURN(-1);
  }
  CCS_RETURN (0);
}


#endif

