// ac_reciprocal_pwl_tb
//
// Tests the ac_reciprocal_pwl() function with various types

// Usage:
// c++ -std=c++11 -I$MGC_HOME/shared/include -DTEST_RECIPROCAL_AC_FIXED ac_reciprocal_pwl_tb.cpp -o test_reciprocal
// ./test_reciprocal 0.00001 0.5 0.0001 4.0

#include "ac_reciprocal_pwl_tb.h"

#include <mc_scverify.h>
#include <math/mgc_ac_math.h>
#include <ac_dsp_utils.h>

using namespace ac_math;

#pragma hls_design top
void CCS_BLOCK(project)(
  const input_type &input,
  output_type &output
)
{
  ac_reciprocal_pwl(input, output);
  //output = ac_reciprocal_pwl<output_type, AC_TRN>(input);

#if 0
  static const ac_fixed<1 + input_type::sign, 1 + input_type::sign, input_type::sign> unity = 1;
  div(unity, input, output);
#endif
}

// ========================= TESTBENCH ====================================================
#if !defined(__SYNTHESIS__) && !defined(__MATLAB_CATAPULT__)

#include <string>
#include <cstring>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

//-----------------------------------------------------------------------------------------
// Support functions for AC_FIXED
// Overloaded function to convert test input data (double) into specific type
template<int W, int I, bool input_S, ac_q_mode input_Q, ac_o_mode input_O>
void double_to_type(
  const double                                                   double_value,
  ac_fixed<W, I, input_S, input_Q, input_O> &  type_value)
{
  type_value = double_value;
}

// Overloaded function to convert type specific test input data to double
template<int W, int I, bool input_S, ac_q_mode input_Q, ac_o_mode input_O>
double type_to_double(
  ac_fixed<W, I, input_S, input_Q, input_O> &  type_value)
{
  return type_value.to_double();
}

//-----------------------------------------------------------------------------------------
// Support functions for AC_FLOAT
// Overloaded function to convert test input data (double) into specific type
template<int W, int I, int input_exp_width, ac_q_mode input_Q>
void double_to_type(
  const double                                                   double_value,
  ac_float<W, I, input_exp_width, input_Q> &   type_value)
{
  type_value = double_value;
}

// Overloaded function to convert type specific test input data to double
template<int W, int I, int input_exp_width, ac_q_mode input_Q>
double type_to_double(
  ac_float<W, I, input_exp_width, input_Q> &   type_value)
{
  return type_value.to_double();
}

//-----------------------------------------------------------------------------------------
// Support functions for AC_COMPLEX
// Overloaded function to convert test input data (double) into specific type
template<class T>
void double_to_type(
  const double double_value,
  ac_complex<T> &type_value)
{
  double_to_type(double_value,type_value.r());
  //double_to_type(double_value,type_value.i());
  double_to_type(2*double_value,type_value.i());
}

// Overloaded function to convert type specific test input data to double
template <class T>
double type_to_double(
  ac_complex<T>                                              &   type_value)
{
  double r = type_to_double(type_value.r());
  double i = type_to_double(type_value.i());
  return sqrt(r*r+i*i);
}


//-----------------------------------------------------------------------------------------
CCS_MAIN(int argc, char *argv[])
{

#ifdef PLOT_RECIPROCAL_FILE_WRITE
  //File to print values to be plotted in matlab
  //Define the "PLOT_RECIPROCAL_FILE_WRITE" macro to start writing to this file.
  const char filename[] = "plot_values.csv";
  std::ofstream outfile(filename);
#endif

#ifdef TEST_RECIPROCAL_RESULT_FILE_WRITE
  //Filename for test outputs.
  //Define the "TEST_RECIPROCAL_RESULT_FILE_WRITE" macro to start writing to this file.
  //The file for test outputs has nine columns
  //Col 1: bitwidth of input
  //Col 2: no of integer bits in input
  //Col 3: Sign (ignore this if using ac_float datatype)
  //Col 4: lower limit of testbench
  //Col 5: upper limit of testbench
  //Col 6: step
  //Col 7: error tolerance
  //Col 8: Maximum error
  //Col 9: Result-PASS or FAIL?
  const char to_fname[] = "test_op.csv";
  std::ofstream tofname;
  tofname.open(to_fname, ios::out | ios::app);
#endif

#ifdef PLOT_RECIPROCAL_FILE_WRITE
  //The outfile has four columns. One contains the input, expressed in double, the second containes the expected output value, with the
  //input taken in double. The third contains the type converted expected value, where the input, expressed in the relevant type, is first
  //converted into double and then the output for that is calculated using standard math functions. The fourth contains the approximate
  //PWL output with the input expressed in the relevant type.
#if defined(TEST_RECIPROCAL_AC_FIXED)
  outfile << "Input (double), Expected Value (double), Expected Value (FixPt), PWL Approximated Value (double), PWL Approximated Value (FixPt), Error(percent)" << endl;
#elif defined(TEST_RECIPROCAL_AC_COMPLEX_AC_FIXED)
  outfile << "Input (double), Expected Value (double), Expected Value (Complex<FixPt>), PWL Approximated Value (double), PWL Approximated Value (Complex<FixPt>), Error(percent)" << endl;
#elif defined(TEST_RECIPROCAL_AC_FLOAT)
  outfile << "Input (double), Expected Value (double), Expected Value (Float), PWL Approximated Value (double), PWL Approximated Value (Float), Error(percent)" << endl;
#elif defined(TEST_RECIPROCAL_AC_COMPLEX_AC_FLOAT)
  outfile << "Input (double), Expected Value (double), Expected Value (Complex<Float>), PWL Approximated Value (double), PWL Approximated Value (Complex<Float>), Error(percent)" << endl;
#else
#error Must select a datatype
#endif

#endif

  input_type  input;
  output_type output;

  double lower_limit, upper_limit, step, allowed_error;
  cout << "============ reciprocal_pwl test ==================" << endl;
  if ( argc < 5 ) {
    // cout << "FILE : " << __FILE__ << ", LINE : " << __LINE__ << ", Warning: using default testbench ranges" << endl;
    lower_limit   = -3;
    upper_limit   = 3;
    step          = 0.0001;
    allowed_error = 4.0;
  } else {
    //First argument : lower limit of testbench.
    //Second argument : Upper limit
    //Third argument : step size - Can be "-" means use one Quantum
    //Fourth argument : tolerance.
    lower_limit   = atof(argv[1]);
    upper_limit   = atof(argv[2]);
    if (strcmp("-", argv[3]) != 0) {
      step          = atof(argv[3]);
    } else {
#if defined(TEST_RECIPROCAL_AC_FIXED) || defined(TEST_RECIPROCAL_AC_FLOAT)
      step = input.template set_val<AC_VAL_QUANTUM>().to_double();
#elif defined(TEST_RECIPROCAL_AC_COMPLEX_AC_FIXED) || defined(TEST_RECIPROCAL_AC_COMPLEX_AC_FLOAT)
      step = input._r.template set_val<AC_VAL_QUANTUM>().to_double();
#else
#error Must select a datatype
#endif
    }
    allowed_error = atof(argv[4]);
  }
  //cout << "FILE : " << __FILE__ << ", LINE : " << __LINE__ << endl;
  cout << "lower_limit    = " << lower_limit << endl;
  cout << "upper_limit    = " << upper_limit << endl;
  cout << "step           = " << step << endl;
  cout << "allowed_error  = " << allowed_error << endl;
  cout << endl;
  double old_real_output;
  bool compare = false;

  ac_channel<double> expectVals;
  ac_channel<double> actualVals;

  std::vector<double> sampleVals;
  for (double i = lower_limit; i < upper_limit; i += step) {
    double_to_type(i, input);

    if (input != 0) {
      CCS_DESIGN(project)(input,output);

      double double_recip   = 1.0/i;
#if defined(TEST_RECIPROCAL_AC_COMPLEX_AC_FIXED) || defined(TEST_RECIPROCAL_AC_COMPLEX_AC_FLOAT)
      // double_recip needs to take care for complex type
      double b = 2*i;  // from type_to_double above
      double_recip = 1/sqrt(i*i+b*b);
#endif
      double expected_value = 1.0/type_to_double(input);
      double actual_value   = type_to_double(output);

      sampleVals.push_back(i);
      expectVals.write(double_recip);
      actualVals.write(actual_value);
      ac_channel<double> tmpExpVals;
      ac_channel<double> tmpActVals;
      tmpExpVals.write(double_recip);
      tmpActVals.write(actual_value);
      int tmpIdx;
      double this_error;
      get_stats(tmpExpVals, tmpActVals, this_error, tmpIdx);
      //double expected_value = 1.0/type_to_double(input);
      //double this_error     = abs( (expected_value - actual_value) / expected_value ) * 100.0;

#if defined(TEST_RECIPROCAL_AC_FIXED) || defined(TEST_RECIPROCAL_AC_FLOAT)
      //Make sure that function is monotonic. Compare old value (value of previous iteration) with current value. Since the reciprocal function we
      //are testing is an increasing function, and our testbench value keeps incrementing or remains the same (in case of saturation), we expect the
      //old value to be greater than or equal to the current one.

      //Also, since the reciprocal function has a large discontinuity at x = 0; we make sure we don't compare values when we cross this point.
      //We do this by checking the signage of the old output vs that of the new output. Since we aren't checking for zero inputs, crossing x = 0
      //will mean that the old output is negative and the new one is positive, in case of an increasing testbench.
      bool sign_same = (old_real_output > 0 && actual_value > 0) || (old_real_output < 0 && actual_value < 0);

      if (compare && sign_same) {
        //Figuring out what the normalized value was for the input is a good way to figure out where the discontinuity occured w.r.t. the PWL segments.
#if defined(TEST_RECIPROCAL_AC_FIXED)
        //Find normalized value of the input value itself
        ac_fixed<input.width, int(input.sign), input.sign, input.q_mode, input.o_mode> norm_input;
        ac_normalize(input, norm_input);
#elif defined(TEST_RECIPROCAL_AC_FLOAT)
        //If the input is a float, find the normalized value of the mantissa of the input.
        ac_fixed<input.m.width, int(input.m.sign), input.m.sign, input.m.q_mode> norm_input;
        ac_normalize(input.m, norm_input);
#endif
        //if by any chance the function output has dropped in value, print out at what point the problem has occured and throw a runtime assertion.
        //also print out what the normalized input value was at that point.
        if (old_real_output < actual_value) {
          //cout << "FILE : " << __FILE__ << ", LINE : " << __LINE__ << endl;
          cout << "Warning : Real, fixed point output not monotonic at :" << endl;
          cout << "x = " << input << endl;
          cout << "y = " << output << endl;
          cout << "old_real_output = " << old_real_output << endl;
          cout << "normalized x    = " << norm_input << endl;
          //assert(false);   //Uncomment if you want the program to stop once monotonicity is violated.
        }
      }
      //Update the variable for old_real_output.
      old_real_output = actual_value;
      //Be setting compare to true, we make sure that once there is an old value stored, we can start comparing for monotonicity.
      compare = true;
#endif

#ifdef DEBUG
      cout << "FILE : " << __FILE__ << ", LINE : " << __LINE__ << endl;
      cout << "input (double) = " << i << endl;
      cout << "input          = " << input << endl;
      cout << "output         = " << output << endl;
      cout << "expected       = " << double_recip << endl;
      cout << "pct error      = " << this_error << endl << endl;
      assert(this_error < allowed_error);
#endif

#ifdef PLOT_RECIPROCAL_FILE_WRITE
      outfile << i << ",  " 
              << double_recip << ",  " 
              << expected_value << ",  " 
              << actual_value << ",  " 
              << output << ",   " 
              << this_error << endl;
#endif
    }
  }

#ifdef PLOT_RECIPROCAL_FILE_WRITE
  //Close file
  outfile.close();
#endif

  double max_error = 0;
  double max_error_sample = 0;
  double max_error_actual = 0;
  double max_error_expected = 0;

  int maxIdx;
  get_stats(expectVals, actualVals,  max_error, maxIdx);

  if (maxIdx != -1) {
    max_error_sample = sampleVals[maxIdx];
    max_error_actual = actualVals[maxIdx];
    max_error_expected = expectVals[maxIdx];
  }

  //Print out: the maximum error, the x-value at which the max. error occured, the pwl output at that x-value as well as the expected output.
  // cout << "FILE : " << __FILE__ << ", LINE : " << __LINE__ << endl;
  cout << "Testbench finished" << endl;
  cout << "max_error =" << max_error << "  with sample =" << max_error_sample 
       << "  expected=" << max_error_expected << "  actual=" << max_error_actual << endl;

  if (max_error > allowed_error) {
    //Print out details regarding the input type, e.g. precision, signedness, the testbench ranges and the max. error, to the output file.
#ifdef TEST_RECIPROCAL_RESULT_FILE_WRITE
#if defined(TEST_RECIPROCAL_AC_FIXED)
    tofname << input.width << "," << input.i_width << "," << input.sign << "," << lower_limit << "," << upper_limit << "," << step << ","
            << allowed_error << "," << max_error << ", FAIL " << endl;
#elif defined(TEST_RECIPROCAL_AC_COMPLEX_AC_FIXED)
    tofname << input.r().width << "," << input.r().i_width << "," << input.r().sign << "," << lower_limit << "," << upper_limit << "," << step << ","
            << allowed_error << "," << max_error << ", FAIL " << endl;
#elif defined(TEST_RECIPROCAL_AC_FLOAT)
    tofname << input.width << "," << input.i_width << "," << input.e_width << "," << lower_limit << "," << upper_limit << "," << step << ","
            << allowed_error << "," << max_error << ", FAIL " << endl;
#elif defined(TEST_RECIPROCAL_AC_COMPLEX_AC_FLOAT)
    tofname << input.r().width << "," << input.r().i_width << "," << input.r().e_width << "," << lower_limit << "," << upper_limit << "," << step << ","
            << allowed_error << "," << max_error << ", FAIL " << endl;
#endif
    tofname.close();
#endif

    // cout << "FILE : " << __FILE__ << ", LINE : " << __LINE__ << endl;
    cout << "Error tolerance of " << allowed_error << " percent error exceeded - FAIL" << endl;
    cout << "===================================================" << endl;
    CCS_RETURN(-1);
  }

#ifdef TEST_RECIPROCAL_RESULT_FILE_WRITE
#if defined(TEST_RECIPROCAL_AC_FIXED)
  tofname << input.width << "," << input.i_width << "," << input.sign << "," << lower_limit << "," << upper_limit << "," << step << ","
          << allowed_error << "," << max_error << ", PASS " << endl;
#elif defined(TEST_RECIPROCAL_AC_COMPLEX_AC_FIXED)
  tofname << input.r().width << "," << input.r().i_width << "," << input.r().sign << "," << lower_limit << "," << upper_limit << "," << step << ","
          << allowed_error << "," << max_error << ", PASS " << endl;
#elif defined(TEST_RECIPROCAL_AC_FLOAT)
  tofname << input.width << "," << input.i_width << "," << input.e_width << "," << lower_limit << "," << upper_limit << "," << step << ","
          << allowed_error << "," << max_error << ", PASS " << endl;
#elif defined(TEST_RECIPROCAL_AC_COMPLEX_AC_FLOAT)
  tofname << input.r().width << "," << input.r().i_width << "," << input.r().e_width << "," << lower_limit << "," << upper_limit << "," << step << ","
          << allowed_error << "," << max_error << ", PASS " << endl;
#endif
  tofname.close();
#endif

  cout << "===================================================" << endl;
  CCS_RETURN(0);
}
#endif















