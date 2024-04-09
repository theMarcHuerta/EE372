//
// Copyright 2018 Mentor Graphics Corporation
//
// All Rights Reserved.
//
// THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
// MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
//

// Include the design function to be tested
#include "fir_filter.h"

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

typedef double                coeffs_t;
typedef vector<STIMULUS_TYPE> samplesVector_t;

// Forward Declarations of utility functions
bool ReadCSV_Coefficients(string filename, COEFF_TYPE coeffs[TAP_COUNT]);
int  ReadCSV_Samples(string filename, samplesVector_t &samples);
bool WriteCSV_Samples(string, samplesVector_t &samples);

//=============================================================================
// Function: main
//   Test the fir_filter() function using data from CSV files
//-----------------------------------------------------------------------------
CCS_MAIN(int argc, char **argv) // required for sc verify flow in Catapult
{
  // define data structure for holding input and output samples:
  samplesVector_t samples;
  samplesVector_t samples_out;

  // define array to hold coefficients
  COEFF_TYPE filterCoeffs[TAP_COUNT];

  // Read coefficients from CSV into array filterCoeffs
  ReadCSV_Coefficients("coefficients.csv", filterCoeffs);

  // read in samples from CSV file
  if (ReadCSV_Samples("samples.csv", samples) < 0) {
    cerr << __FILE__ << ":" << __LINE__ << " - Failed to read input samples" << endl;
    return -1;
  }

  // Loop through samples, applying them to the filter
  for (vector<STIMULUS_TYPE>::iterator it = samples.begin(); it != samples.end(); ++it) {
    STIMULUS_TYPE stimulus_element = *it;
    CCS_DESIGN(fir_filter)(stimulus_element.i_sample, filterCoeffs, stimulus_element.o_sample);
    samples_out.push_back(stimulus_element);
  }

  WriteCSV_Samples("filter_output.csv", samples_out);

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
int ReadCSV_Samples(string filename, samplesVector_t &samples)
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
    STIMULUS_TYPE stimulus_element;
    string s_time(rowFields[0]);
    stringstream(s_time) >> stimulus_element.time;
    string s_i_sample(rowFields[2]);
    double double_i_sample;
    stringstream(s_i_sample) >> double_i_sample;
    ac_fixed<X_TYPE::width,X_TYPE::i_width,X_TYPE::sign,AC_RND,AC_SAT_SYM> fixed_i_sample = double_i_sample;
    stimulus_element.i_sample = fixed_i_sample;
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
bool WriteCSV_Samples(string oFileName, samplesVector_t &samples)
{
  // create output csv file with results:
  ofstream oSampleFile;
  cout << __FILE__ << ":" << __LINE__ << " - Writing output csv file to '" << oFileName << "'." << endl;
  oSampleFile.open(oFileName.c_str());
  if (!oSampleFile.is_open()) {
    cerr << __FILE__ << ":" << __LINE__ << " - CSV output file '" << oFileName << "' could not be created." << endl;
    return false;
  }
  oSampleFile << "Time" << "," << "o_sample" << endl;
  for (samplesVector_t::iterator it = samples.begin(); it != samples.end(); ++it) {
    oSampleFile << (*it).time << "," << (*it).o_sample << endl;
  }
  oSampleFile.close();
  return true;
}

