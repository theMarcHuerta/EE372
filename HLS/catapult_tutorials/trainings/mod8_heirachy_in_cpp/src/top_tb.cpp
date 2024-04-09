//
// Copyright 2018 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
//

//============================================================================
// Name        : Sample.cpp
// Author      : Jim Corona
// Version     : 2017.06.30
// Copyright   : (c)JimCoronaPE
// Description : Hello World in C++, Ansi-style
//============================================================================

// Include the design function to be tested
#include "top.h"

// Include utility headers
#include <iostream>
#include "csvparser.h"
#include <vector>
#include <assert.h>
#include <string>
#include <sstream>
#include <fstream>
#include <mc_scverify.h>
using namespace std;

// Define types used by this program
struct STIMULUS_TYPE {
  double time;
  X_TYPE i_sample;
  Y_TYPE o_sample;
};

typedef double          coeffs_t;
typedef struct {
  double   time;
  X_TYPE   data;
} i_sampleType;
typedef struct {
  double   time;
  Y_TYPE   data;
} o_sampleType;

typedef vector<i_sampleType>  i_samplesVector_t;
typedef vector<o_sampleType>  o_samplesVector_t;

// Forward Declarations of utility functions
bool ReadCSV_Coefficients(string filename, COEFF_TYPE coeffs[TAP_COUNT]);
int  ReadCSV_Samples(string filename, i_samplesVector_t &samples);
bool WriteCSV_Samples(string, o_samplesVector_t &samples);

//=============================================================================
// Function: main
//   Test the fir_filter() function using data from CSV files
//-----------------------------------------------------------------------------
CCS_MAIN(int argc, char **argv)
{
  cout << __FILE__ << "," << __LINE__ << ":Hierarchical Design of FIR filter with decimator testbench" << endl;
  const RATE_TYPE   M = 3;
  // define channels for DUT connection:
  static ac_channel<MPU_STREAM_TYPE>  mpu_stream_in; // stream into DUT for writing/reading FIR coefficients
  static ac_channel<COEFF_TYPE>    mpu_stream_out; // stream returning from DUT with coefficients read
  static ac_channel<X_TYPE> x_chan;               // inbound sample stream
  static ac_channel<Y_TYPE> y_chan;               // outbound sample stream

  i_samplesVector_t i_samples;                 // input samples data structure
  o_samplesVector_t o_samples;                 // output samples data structure
  COEFF_TYPE        filterCoeffs[TAP_COUNT];      // FIR coefficients data structure

  // Read coefficients from CSV file into array filterCoeffs:
  ReadCSV_Coefficients("coefficients.csv", filterCoeffs);

  // load up coefficient writes into mpu data stream with coefficient writes:
  COEFF_WRITES:for (unsigned i=0; i<TAP_COUNT; i++) {
    MPU_STREAM_TYPE   mpu_request;
    mpu_request.read = false;
    mpu_request.addr = i;
    mpu_request.data = filterCoeffs[i];
    mpu_stream_in.write(mpu_request);
  }
  cout << __FILE__ << "," << __LINE__ << ":" << TAP_COUNT << " coefficient writes are being streamed to DUT..." << endl;

  // load up coefficient reads into mpu data stream to follow writes:
  COEFF_READS:for (unsigned i=0; i<TAP_COUNT; i++) {
    MPU_STREAM_TYPE mpu_request;
    mpu_request.read = true;
    mpu_request.addr = i;
    mpu_stream_in.write(mpu_request);
  }
  cout << __FILE__ << "," << __LINE__ << ":" << TAP_COUNT << " coefficient reads are being streamed to DUT..." << endl;

  // read in x samples from CSV file:
  if (ReadCSV_Samples("samples.csv", i_samples) < 0) {
    cerr << __FILE__ << ":" << __LINE__ << " - Failed to read input samples" << endl;
    return -1;
  }
  cout << __FILE__ << "," << __LINE__ << " - Read in " << i_samples.size() << " input samples." << endl;

  // Loop through samples, streaming each one into the x-channel:
  X_SAMPLE_WRITE:for (i_samplesVector_t::iterator it = i_samples.begin(); it != i_samples.end(); ++it) {
    i_sampleType sample;
    sample = (*it);
    x_chan.write(sample.data);
  }
  cout << __FILE__ << "," << __LINE__ << ":" << i_samples.size() << " inbound samples are being streamed to DUT..." << endl;

  // all input data streams have been loaded, call DUT:
  cout << __FILE__ << "," << __LINE__ << ":Calling DUT with " <<
       mpu_stream_in.size() << " transactions in mpu_stream_in, and " <<
       x_chan.size() << " x samples in x_chan." <<
       endl;

  CCS_DESIGN(top) (x_chan, M, y_chan, mpu_stream_in, mpu_stream_out);
  cout << __FILE__ << "," << __LINE__ << ":DUT has returned with " <<
       mpu_stream_out.size() << " transactions in mpu_stream_out, and " <<
       y_chan.size() << " y samples in y_chan." <<
       endl;

  // sanity checks:
  assert(mpu_stream_out.size()==TAP_COUNT);    // mpu_stream_out should contain all our read data
  assert(mpu_stream_in.size()==0);
  assert(x_chan.size()==0);
  assert(y_chan.size()==i_samples.size()/M);

  COEFF_CHECK:for (unsigned i=0; i<TAP_COUNT; i++) {
    COEFF_TYPE  mpu_out_data;
    mpu_stream_out.read(mpu_out_data);
    assert(mpu_out_data==filterCoeffs[i]);    // make sure we read what we wrote
  }

  Y_SAMPLE_READ:for (unsigned i=0; y_chan.available(1); i++) {
    o_sampleType   y;
    i_sampleType   x = i_samples.at(i*M);     // assign correct timestamp
    y.data = y_chan.read();
    y.time = x.time;
    o_samples.push_back(y);
  }

  WriteCSV_Samples("filter_output.csv", o_samples);  // write output samples, with timestamps, to CSV file

  cout << __FILE__ << ":" << __LINE__ << " - End of testbench." << endl;
  CCS_RETURN(0);
}

//=============================================================================
// Function: ReadCSV_Coefficients
//   Read testbench coefficients from a CSV file formatted as
//     Index,Coefficient
//   Returns true on success, false on error
//   Warns if number of values read exceeds size of the array (and the new
//   values overwrite back at the beginning of the array).
//-----------------------------------------------------------------------------
bool ReadCSV_Coefficients(string filename, COEFF_TYPE coeffs[TAP_COUNT])
{
  unsigned i =  0;
  CsvParser  *csvparser = CsvParser_new("coefficients.csv", ",", 1);
  CsvRow   *row;
  const CsvRow *header = CsvParser_getHeader(csvparser);

  if (header == NULL) {
    printf("%s\n", CsvParser_getErrorMessage(csvparser));
    return false;
  }
  const char **headerFields = CsvParser_getFields(header);
  while ((row = CsvParser_getRow(csvparser)) ) {
    const char **rowFields = CsvParser_getFields(row);
    string s_coeff(rowFields[1]);
    double double_coeff;
    stringstream (s_coeff) >> double_coeff;
    ac_fixed<COEFF_TYPE::width,COEFF_TYPE::i_width,COEFF_TYPE::sign,AC_RND> fixed_coeff = double_coeff;

    coeffs[i%TAP_COUNT] = fixed_coeff;
    CsvParser_destroy_row(row);
    i++;
    if (i>TAP_COUNT) {
      cerr << __FILE__ << ":" << __LINE__ << " - Too many coefficients found in CSV file. Expected " << TAP_COUNT << " values." << endl;
    }
  }
  CsvParser_destroy(csvparser);
  cout << __FILE__ << ":" << __LINE__ << " - Read in " << i << " coefficients from '" << filename << "'" << endl;
  return (i==TAP_COUNT);
}

//=============================================================================
// Function: ReadCSV_Samples
//   Reads testbench sample data from a CSV file formatted as
//     Time,F1,F2,F1 + F2,o_sample
//   Values are returned in the vector passed by reference.
//   Returns -1 on error, else returns the number of samples read in.
//-----------------------------------------------------------------------------
int ReadCSV_Samples(string filename, i_samplesVector_t &samples)
{
  CsvParser  *csvparser = CsvParser_new(filename.c_str(), ",", 1);
  CsvRow   *row;
  const CsvRow *header = CsvParser_getHeader(csvparser);

  if (header == NULL) {
    cerr << CsvParser_getErrorMessage(csvparser) << endl;
    return -1;
  }
  // CSV file is expected to have 4 columns: Time,F1,F2,F1 + F2
  assert(CsvParser_getNumFields(header)==3);

  const char **headerFields = CsvParser_getFields(header);
  while ((row = CsvParser_getRow(csvparser)) ) {
    const char **rowFields = CsvParser_getFields(row);
    double time;
    i_sampleType stimulus_element;
    string s_time(rowFields[0]);
    stringstream(s_time) >> stimulus_element.time;
    string s_i_sample(rowFields[2]);
    double double_i_sample;
    stringstream(s_i_sample) >> double_i_sample;
    ac_fixed<X_TYPE::width,X_TYPE::i_width,X_TYPE::sign,AC_RND,AC_SAT_SYM>  fixed_i_sample = double_i_sample;
    stimulus_element.data = fixed_i_sample;
    samples.push_back(stimulus_element);
    CsvParser_destroy_row(row);
  }
  cout << __FILE__ << ":" << __LINE__ << " - CSV file '" << filename << "' " << samples.size() << " samples were read in." << endl;
  CsvParser_destroy(csvparser);
  return samples.size();
}

//=============================================================================
// Function: WriteCSV_Samples
//   Writes testbench output sample data to a CSV file formatted as
//     Time,o_sample
//-----------------------------------------------------------------------------
bool WriteCSV_Samples(string oFileName, o_samplesVector_t &samples)
{
  // create output csv file with results:
  ofstream oSampleFile;
  cout << __FILE__ << ":" << __LINE__ << " - Writing output csv file to '" << oFileName << "'." << endl;
  oSampleFile.open(oFileName.c_str());
  if (!oSampleFile.is_open()) {
    cerr << __FILE__ << ":" << __LINE__ << " - CSV output file '" << oFileName << "' could not be created." << endl;
    return false;
  }
  oSampleFile << "Time" << "," << "o_sample.time" << endl;
  for (o_samplesVector_t::iterator it = samples.begin(); it != samples.end(); ++it) {
    o_sampleType   sample;
    sample = (*it);
    oSampleFile << sample.time << "," << sample.data << endl;
  }
  oSampleFile.close();
  return true;
}

