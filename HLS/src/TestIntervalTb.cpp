#include <iostream>

#include "TestInterval.cpp"

using namespace std;

CCS_MAIN(int argc, char** argv) {

    TestInterval inst; // DUT

    sfp_11_22 lowest; // output
    sfp_11_22 highest; // output

    inst.run(lowest, highest);

    cout << lowest << endl;
    cout << highest << endl;
    cout << "Done!  " << endl;

    CCS_RETURN(0);
}