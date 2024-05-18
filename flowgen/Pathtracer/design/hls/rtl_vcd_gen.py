import subprocess
import sys
import inspect 
import argparse
import json
import time

CRED = '\033[91m'
CGREEN  = '\33[32m'
CEND = '\033[0m'

def test_rtl_test():
    print("Running rtl_test with top level Pathtracer!")

    # Clean previous builds
    try:
        subprocess.run(['make', 'clean'], check=True)
    except subprocess.CalledProcessError as e:
        print(f"Error during cleaning build: {e}")
        return 0, 0

    # Run the specific make command to ensure all necessary files are generated
    try:
        subprocess.run(['make', 'Pathtracer'], check=True)  # Use the correct make target
    except subprocess.CalledProcessError as e:
        print(f"Build failed: {e}")
        return 0, 0

    # ----------------------------------------------------------------------------------
    # BEGIN SCRIPT FOR CREATING VCD FILE
    # ----------------------------------------------------------------------------------

    # script for producing vcd file output
    filename = '../../build/1-hls/build/Pathtracer.v1/concat_sim_rtl.v'  # replace with your Verilog file name

    # read the Verilog file
    with open(filename, 'r') as f:
        contents = f.readlines()

    # find the index of the last occurrence of "endmodule"
    endmodule_index = len(contents) - 1
    while endmodule_index >= 0:
        if 'endmodule' in contents[endmodule_index]:
            break
        endmodule_index -= 1

    # insert the lines before the last occurrence of "endmodule"
    if endmodule_index >= 0:
        insert_lines = [
            'initial begin\n',
            '  $dumpfile("Pathtracer.vcd");\n',
            '  $dumpvars(0, Pathtracer);\n',
            '  #50000;\n',
            '  $finish(2);\n',
            'end\n'
        ]
        contents.insert(endmodule_index, '\n'.join(insert_lines))

    # write the updated Verilog file
    with open(filename, 'w') as f:
        contents = ''.join(contents)
        f.write(contents)

    # ----------------------------------------------------------------------------------
    # END SCRIPT FOR CREATING VCD FILE
    # ----------------------------------------------------------------------------------

    # Run the specific test
    try:
        result = subprocess.run(['make', 'Pathtracer_rtl_test'], check=True)
        if result.returncode == 0:
            print("Test completed successfully. WOOOOOOOOOOOOOOOO.")
            return 1, 1  # Assuming 'run' and 'passed' should be incremented upon success
        else:
            print("Test failed with return code:", result.returncode)
            return 1, 0  # Assuming 'run' is incremented but 'passed' is not
    except subprocess.CalledProcessError as e:
        print(f"Error during test execution: {e}")
        return 1, 0  # Assuming 'run' is incremented but 'passed' is not


def test_c_test():
    run = 0
    passed = 0
    print("Running C test for Pathtracer")

    # Define ANSI color codes for terminal output
    CGREEN = '\033[92m'  # Green text
    CRED = '\033[91m'   # Red text
    CEND = '\033[0m'    # Reset text color

    # Run the make command for the Pathtracer C test
    try:
        process = subprocess.run(['make', 'Pathtracer_c_test'], check=True)
        if process.returncode == 0:
            print(CGREEN + "Pathtracer C Test passed!\n" + CEND)
            passed += 1
        else:
            print(CRED + "Pathtracer C Test failed\n" + CEND)
    except subprocess.CalledProcessError as e:
        print(CRED + f"Pathtracer C Test failed with an error: {e}\n" + CEND)
    except Exception as e:
        print(CRED + f"An error occurred: {e}\n" + CEND)

    run += 1  # Increment the number of runs

    return run, passed


# Argument parser setup
parser = argparse.ArgumentParser(description='HLS RUN')
parser.add_argument('tests', type=str, nargs='*', default=['all'],
                    help='list of tests you would like to run')

args = parser.parse_args()

# Find all test functions in the current module
all_tests = [obj for name,obj in inspect.getmembers(sys.modules[__name__]) 
                        if (inspect.isfunction(obj) and 
                            name.startswith('test') and
                            obj.__module__ == __name__)]

# Determine which tests to run based on arguments
tests = []
if "all" in args.tests:
    tests = all_tests
else:
    for arg in args.tests:
        tests.extend(test for test in all_tests if arg in test.__name__)

# Print the names of tests to be run
tests_names = [test.__name__ for test in tests]
print("Tests being run: ", tests_names)

# Initialize test counts
tests_run = 0
tests_passed = 0

# Run tests and measure time
start = time.time()
for test in tests:
    run, passed = test()
    tests_run += run
    tests_passed += passed
end = time.time()

# Print summary of test results
print("Tests passed:", tests_passed)
print("Tests run:", tests_run)
print("Elapsed time: ", (end - start))