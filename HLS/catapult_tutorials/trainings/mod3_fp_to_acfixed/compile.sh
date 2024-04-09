#!/bin/sh

if [[ -z "${MGC_HOME}" ]]; then
  echo "MGC_HOME not set.  You must set \$MGC_HOME to the Catapult install tree."
  exit;
fi

if [ ! -f src/fir_filter.cpp ]; then
  echo "fir_filter.cpp not found.  You must run this script from the ./module3/ directory."
  exit;
fi

echo "compiling files..."
$MGC_HOME/bin/g++ -c -o src/csvparser.o $MGC_HOME/shared/include/csvparser.c >&2
$MGC_HOME/bin/g++ -c -o src/fir_filter.o src/fir_filter.cpp -I"$MGC_HOME/shared/include"
$MGC_HOME/bin/g++ -c -o src/fir_filter_tb.o src/fir_filter_tb.cpp -I"$MGC_HOME/shared/include"
$MGC_HOME/bin/g++ -o fir_filter_tb src/csvparser.o src/fir_filter.o src/fir_filter_tb.o 

echo "Compilation complete.  Execute 'fir_filter_tb' to run the FIR filter"

