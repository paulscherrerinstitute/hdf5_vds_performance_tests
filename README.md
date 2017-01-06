# HDF5 Virtual dataset performance tests
With this repo we are trying to evaluate the performance of the virtual dataset feature inside HDF5.

## Performance results
The latest performance results in CSV format are available in the **results/** folder.

The [Jupyter notebook](results/analysis.ipynb) containing the analysis of the test results is also available in the **results/** folder.

## Compiling
To compile the tests, first make sure that the **HDF_INSTALL** variable in the **Makefile** is set to your local HDF5 installation. After that you can compile the tests by running

```
make
```

from the project root directory. The executables will be placed in the **bin/** subdirectory.

In addition to the flags set in the Makefile, you can pass additional one to modify the behavior of the tests:

- **DEBUG**: Print additional info to the console. This is meant primarily to see if the tests are working correctly. It **should not** be used when measuring performance.
- **HUMAN**: The output of the tests will be in more human friendly format. This flag **should not** be set when running tests with the bash scripts - see "Running the tests".

To set this flags append the *options* key to the make command:

```
make options="-DDEBUG -DHUMAN"
```

## Running the tests
The tests are divided into 2 executables:

- **hdf5_test_prepare**: Prepare the files needed to execute the tests. This should be run first, but only if the test files are not already available.
- **hdf5_test_execute**: Execute the tests on the prepared files.

Both executables should be run with the exact same parameters. To get a list of parameters and some help, run the executable without any parameters. This will also give you the basic example on how to run each executable.

To facilitate measurement, test runners are available in the form of a bash scripts named **[name of test]\_runner.sh**. Each script has some pre-set variables that define how the test will be executed.
The results of the tests executed from the bash scripts are stored in an output CSV file in the results/ folder. When running the tests from the bash scripts, executables have to be compiled without any additional flags.

## Performing measurements
In order to perform the tests with the current input parameters, 128GB of free disk space is recommended.

The complete procedure to run all currently available tests is the following:

```
make
sh timeseries_runner.sh
sh 2d_tile_runner.sh
sh 3d_tile_runner.sh
sh gigafrost_timeseries_runner.sh
sh gigafrost_tile_runner.sh
```

The results in CSV format will be saved into the **results/** folder.

The generated test files will be saved into the **temp/** folder (the folder will be created if it does not exist).
