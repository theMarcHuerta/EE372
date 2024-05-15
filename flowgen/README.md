First, make sure to source the setup.csh file.

Next, enter into the build directory of the design you want to run, and run the following:

`mflowgen run --design ../design/`

# Helpful make Targets
*  `make list` - list all the nodes in the graphs and their corresponding step number
*  `make status` - list the build status of all the steps
*  `make graph` - generates a PDF of the graph
*  `make N` - runs step N
*  `make clean-N` - removes the folder for step N, and sets the status of steps [N,) to build
*  `make clean-all` - removes folders for all the steps
*  `make runtimes` - lists the runtime for each step
