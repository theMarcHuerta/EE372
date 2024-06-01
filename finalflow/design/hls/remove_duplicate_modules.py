# input text file
inputFile = "../../build/1-hls/build/Pathtracer.v1/concat_rtl.v"
# output text file
outputFile = "../../build/1-hls/build/Pathtracer.v1/concat_rtl_trimmed.v"
# Opening the given file in read-only mode.
with open(inputFile, 'r') as inputfiledata, open(outputFile, 'w') as outputfiledata:
   # Read the file lines using readlines()
   inputFilelines = inputfiledata.readlines()
   # storing the current line number in a variable
   lineindex = 1
   # count number of unreg_hier module instantiations
   numUnregHeir = 0
   # boolean indicating if line should be kept
   print = 1
   # iterate through lines
   for line in inputFilelines:
      if (line.__contains__("module unreg_hier")):
        if (numUnregHeir == 0):
            numUnregHeir += 1
        else:
           print = 0
      if (print == 1):
         outputfiledata.write(line) 
      if (line.__contains__("endmodule")):
         print = 1
# Closing the files
inputfiledata.close()
outputfiledata.close()