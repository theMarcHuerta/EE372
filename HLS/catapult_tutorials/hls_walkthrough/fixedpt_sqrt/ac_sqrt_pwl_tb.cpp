// ac_sqrt_pwl_tb
//
// Tests the sqrt_pwl() function with various types

// Usage:
// g++ -I$MGC_HOME/shared/include -DTEST_SQRT_AC_FIXED ac_sqrt_pwl_tb.cpp -o test_sqrt
// test_sqrt 0.00001 0.5 0.0001

//include the header file of testbench
#include "ac_sqrt_pwl_tb.h"
using namespace ac_math;

//sc_verify header file
#include <mc_scverify.h>
//defining top design and using CCS_BLOCK to declare the design
#pragma hls_design top
void CCS_BLOCK(project)(
    input_type &input,
    output_type &output
)
{
    ac_sqrt_pwl(input, output); //can be replace by, output = ac_sqrt_pwl <output_type> (input);
} 

// ========================= TESTBENCH ====================================================
//outside synthesis
#if !defined(__SYNTHESIS__) && !defined(__MATLAB_CATAPULT__)

//math library of c++
#include <math.h> 
#include <stdlib.h>
#include <cstring>
#include <fstream>
#include <sstream>
//activate this macro for debug mode
// #define DEBUG 
#include <iostream> //iostream and std for cout statements
using namespace std;

#include <ac_dsp_utils.h>

//-----------------------------------------------------------------------------------------
// Support functions for AC_FIXED
// Overloaded function to convert test input data (double) into specific type
template<int input_width, int input_int, bool input_S, ac_q_mode input_Q, ac_o_mode input_O>
void double_to_type(
    double                                                   double_value,
    ac_fixed<input_width, input_int, input_S, input_Q, input_O> &  type_value)
{
    type_value = double_value;
}

//-----------------------------------------------------------------------------------------
// Support functions for AC_FLOAT
// Overloaded function to convert test input data (double) into specific type
template<int input_width, int input_int, int input_exp_width, ac_q_mode input_Q>
void double_to_type(
    const double                                                   double_value,
    ac_float<input_width, input_int, input_exp_width, input_Q> &   type_value)
{
    type_value = double_value;
}

//-----------------------------------------------------------------------------------------
// Support functions for AC_COMPLEX of AC_FIXED
// Overloaded function to convert test input data (double) into specific type
template<class T>
void double_to_type(
    const double                                                   double_value,
    ac_complex<T>                                              &   type_value)
{
    double_to_type(double_value,type_value.r());
    double_to_type(double_value,type_value.i());
}

// Overloaded function to take specific test input complex data and return its mod
template <class T>
ac_complex<double> type_to_double(
    ac_complex<T>                                              &   type_value)
{
    ac_complex <double> return_val;
    return_val.r() = type_value.r().to_double();
    return_val.i() = type_value.i().to_double();
    return return_val;
}


//Implementation to compute square root of complex numbers, passed using ac_complex wrapper and double as C++ datatype
//Formula for this computation is given by:
//
// output_real_part = sqrt ((sqrt(mod) + input_real_part)/2)
// output_imaginary_part = sqrt ((sqrt(mod) - input_real_part)/2)

// where mod is given by, sqrt (input_real_part * input_real_part + input_imaginary_part * input_imaginary_part)
ac_complex <double> sqrt (ac_complex <double> input)
{
    ac_complex <double> output;
    double mod = input.r()*input.r() + input.i()*input.i();
    //cout << "mod = " << mod << endl;
    //cout << "Sqrt = " << sqrt (mod) << endl;
    output.r() = sqrt ((sqrt(mod) + input.r())/2);
    output.i() = sqrt ((abs (-sqrt(mod) + input.r()))/2);
    //cout << "output.i() " << output.i() << endl;
    double y_neg = -output.i();
    //handling sign of square root of complex number
    output._i = (input._i) < 0? y_neg : output.i();
    //cout << "output : " << output << endl;
    return output;
}

//Main method which is combined implemenation of testbench
CCS_MAIN(int argc, char *argv[])
{
  //File to print values to be plotted in matlab
#ifdef PLOT_SQRT_FILE_WRITE
  const char filename[] = "plot_values.csv";
  std::ofstream outfile(filename);
#endif

  //Filename for test outputs.
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
  
  // const char to_fname[] = "test_op.csv";
  // std::ofstream tofname;
  // tofname.open(to_fname, ios::out | ios::app);

  //The outfile has four columns. One contains the input, expressed in double, the second containes the expected output value, with the
  //input taken in double. The third contains the type converted expected value, where the input, expressed in the relevant type, is first
  //converted into double and then the output for that is calculated using standard math functions. The fourth contains the approximate
  //PWL output with the input expressed in the relevant type.
#ifdef PLOT_SQRT_FILE_WRITE
  outfile << "Input(double), Input(FixPt),  Expected(double), PWL Value(double), PWL Value(FixPt), Error(percent)" << std::endl;
#endif

  double lower_limit, upper_limit, step, allowed_angle_error, allowed_magnitude_error;
  input_type  input;
  output_type output;

  int nArgs = 5;
#ifdef TEST_SQRT_AC_COMPLEX_AC_FIXED
  nArgs = 6;
#endif
  
  cout << "============ sqrt_pwl test ==================" << endl;
  //when arguments provided aren't sufficient enough, go here
  if ( argc < nArgs ) {
    cout << "Warning: using default testbench ranges" << endl;
    lower_limit   = 0;
    upper_limit   = 1200;
    step          = 0.5;
    allowed_angle_error = 1.0;  //put default errror as 1 degeees, expected error in angle for complex numbers 
    allowed_magnitude_error = 1.0; // default magnitude error is 1 percent
    allowed_angle_error = 1.0;  //put default errror as 1 degeees, expected error in angle for complex numbers 
  } 
  else {
    lower_limit   = atof(argv[1]);
    upper_limit   = atof(argv[2]);
    if (strcmp("-", argv[3]) != 0) {
      step          = atof(argv[3]);
    } else {
#if defined(TEST_SQRT_AC_FIXED) || defined(TEST_SQRT_AC_FLOAT)
      step = input.template set_val<AC_VAL_QUANTUM>().to_double();
#elif defined(TEST_SQRT_AC_COMPLEX_AC_FIXED) 
      step = input._r.template set_val<AC_VAL_QUANTUM>().to_double();
#else
#error Must select a datatype
#endif
    }
    allowed_magnitude_error = atof(argv[4]);  // use this for float and fixed
    allowed_angle_error = 0;
#ifdef TEST_SQRT_AC_COMPLEX_AC_FIXED
    allowed_angle_error = atof(argv[5]);     // Needed for complex
#endif
  }
  //Print out the limits at which the testbench is operating alongside step and allowed error via transcript
  cout << "lower_limit    = " << lower_limit << endl;
  cout << "upper_limit    = " << upper_limit << endl;
  cout << "step           = " << step << endl;
#ifdef TEST_SQRT_AC_COMPLEX_AC_FIXED
  cout << "allowed_magnitude_error  = " << allowed_magnitude_error << endl;
  cout << "allowed_angle_error  = " << allowed_angle_error << endl;
#else
  cout << "allowed_error  = " << allowed_magnitude_error << endl;
#endif
  cout << endl;
   
  ac_channel<double> expectVals;
  ac_channel<double> actualVals;

  std::vector<double> sampleVals;
  std::vector<double> history;
#ifdef TEST_SQRT_AC_COMPLEX_AC_FIXED
  double angleErrExpect = 0;
  double angleErrActual = 0;
  double angleErr;
  double maxAngleErr = -1;
  
  std::vector<double> angleErrs;
  int maxAngleErrIdx = -1;
#endif

  for (double i = lower_limit; i < upper_limit; i += step) {
    sampleVals.push_back(i);
    double_to_type(i, input);
    CCS_DESIGN(project)(input,output);

    double actual_value;
    double expected_value;
    ac_channel<double> tmpExpVals;
    ac_channel<double> tmpActVals;

    //===========================================================================================================================
    //  Fixed and Floating point implementation
    //===========================================================================================================================
#if defined (TEST_SQRT_AC_FIXED) || defined (TEST_SQRT_AC_FLOAT)
    actual_value   = output.to_double();
    expected_value = sqrt (input.to_double());
#endif

    //===========================================================================================================================
    //  Complex of Fixed point implementation
    //===========================================================================================================================
#if defined (TEST_SQRT_AC_COMPLEX_AC_FIXED)
    ac_complex<double> expTyped/*expected_value*/  = sqrt (type_to_double(input));

    //compute the error
    double angle_double, magnitude_double;
    double angle_actual, magnitude_actual;
#define PI 3.14159265
    
    expected_value = expTyped.r() * expTyped.r() + expTyped.i() * expTyped.i();
    actual_value   = (output.r() * output.r() + output.i() * output.i()).to_double();
    
    angle_double = atan (expTyped.r() / expTyped.i());
    angle_actual = atan (output.r().to_double() / output.i().to_double());
    
    angleErr = abs (angle_double - angle_actual) * PI/180;
    
    if (angleErr > maxAngleErr) {
      maxAngleErr = angleErr;
      angleErrExpect = angle_double;
      angleErrActual = angle_actual;
      maxAngleErrIdx = angleErrs.size();
    }
    angleErrs.push_back(angleErr);

#endif

    expectVals.write(expected_value);
    actualVals.write(actual_value);
    tmpExpVals.write(expected_value);
    tmpActVals.write(actual_value);
    int tmpIdx;
    double thisErr;
    get_stats(tmpExpVals, tmpActVals, thisErr, tmpIdx, &history);
    
    std::vector<double>::iterator it = history.begin();
    history.insert(it, thisErr);
    if (history.size() > 2) {
      history.resize(2);
    }
    //===========================================================================================================================

    //Debug macro to print the debug statements
#ifdef DEBUG
    cout << "input (double) = " << i << endl;
    cout << "input          = " << input << endl;
    cout << "output         = " << output << endl;
    cout << "expected output = " << expected_value << endl;
    cout << "error           = " << thisErr << endl;
#if defined (TEST_SQRT_AC_COMPLEX_AC_FIXED)
    cout << "angle error      = " << angleErr << endl;
#endif
#endif

#ifdef PLOT_SQRT_FILE_WRITE
    outfile << "  " << i << ",   " << input << ",   " << expected_value << ",   " << output.to_double() << ",   " << output << ",   " << thisErr << std::endl;
#endif
  }

#ifdef PLOT_SQRT_FILE_WRITE
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
  cout << "Testbench finished" << endl;
  cout << "max_error =" << max_error << "  with sample =" << max_error_sample 
       << "  expected=" << max_error_expected << "  actual=" << max_error_actual << endl;

#if defined(TEST_SQRT_AC_COMPLEX_AC_FIXED)
  max_error_sample = sampleVals[maxAngleErrIdx];
  cout << "max_angle_error =" << max_error << "  with sample =" << max_error_sample 
       << "  expected=" << angleErrExpect << "  actual=" << angleErrActual << endl;
#endif

  //If error exceeds the allowed error then write that the test failed
  if (max_error > allowed_magnitude_error) {
#if defined(TEST_SQRT_AC_FIXED)
    //tofname << input.width << "," << input.i_width << "," << input.sign << "," << lower_limit << "," << upper_limit << "," << step << ","
    // << allowed_magnitude_error << "," << max_magnitude_error << ", FAIL " << endl;
#elif defined(TEST_SQRT_AC_COMPLEX_AC_FIXED)
    // tofname << input.r().width << "," << input.r().i_width << "," << input.r().sign << "," << lower_limit << "," << upper_limit << "," << step << ","
    // << allowed_magnitude_error << "," << max_magnitude_error << ", FAIL " << endl;
#elif defined(TEST_SQRT_AC_FLOAT)
    // tofname << input.width << "," << input.i_width << "," << input.e_width << "," << lower_limit << "," << upper_limit << "," << step << ","
    // << allowed_magnitude_error << "," << max_magnitude_error << ", FAIL " << endl;
#endif
    // tofname.close();
    cout << "Error tolerance value" << allowed_magnitude_error << " percent error exceeded - FAIL" << endl;
    cout << "===================================================" << endl;
    CCS_RETURN(-1);
  }
#if defined(TEST_SQRT_AC_COMPLEX_AC_FIXED)
  else if (maxAngleErr > allowed_angle_error) {
    // tofname << input.r().width << "," << input.r().i_width << "," << input.r().sign << "," << lower_limit << "," << upper_limit << "," << step << ","
    // << allowed_angle_error << "," << max_angle_error << ", FAIL " << endl;
    // tofname.close();
    cout << "Error tolerance of Angle values for complex numbers" << allowed_angle_error << " percent error exceeded - FAIL" << endl;
    cout << "===================================================" << endl;
    CCS_RETURN(-1);
  }
#endif

  //if error didn't exceed maximum allowed error then, write that test was succeeded and return 0 (to indicate that verification succeded).
#if defined(TEST_SQRT_AC_FIXED)
  // tofname << input.width << "," << input.i_width << "," << input.sign << "," << lower_limit << "," << upper_limit << "," << step << ","
  // << allowed_magnitude_error << "," << max_magnitude_error << ", PASS " << endl;
#elif defined(TEST_SQRT_AC_COMPLEX_AC_FIXED)
  // tofname << input.r().width << "," << input.r().i_width << "," << input.r().sign << "," << lower_limit << "," << upper_limit << "," << step << ","
  // << allowed_magnitude_error << "," << max_magnitude_error <<  "," << allowed_angle_error << "," << max_angle_error  << ", PASS " << endl;
#elif defined(TEST_SQRT_AC_FLOAT)
  // tofname << input.width << "," << input.i_width << "," << input.e_width << "," << lower_limit << "," << upper_limit << "," << step << ","
  // << allowed_magnitude_error << "," << max_magnitude_error << ", PASS " << endl;
#endif
  // tofname.close();
  
  cout << "===================================================" << endl;
  CCS_RETURN(0);
}
#endif

