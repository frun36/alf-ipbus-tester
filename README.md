# ALF IPbus tester
This repository contains the framework used to thoroughly test [ALF IPbus](https://github.com/VictorPierozak/ALFIPbus), a temporary solution for migrating the FIT Detector Control System to the standard ALFRED framework developed for ALICE, CERN.

## Getting started
### Overview
The framework consists of two tools: `mock` and `generator`, both configured by one [TOML](https://toml.io/en/) file (see [example_config.toml](example_config.toml)).

#### `mock`
The mock simulates an IPbus slave device. Its register map can be configured by a `.csv` file (see [build/ftm_registers.csv](build/ftm_registers.csv)). It supports:
- reading/writing to appropriate registers
- response randomisation, for simulating failure
- tracking the communication according to a TOML config file
- logging in normal or verbose mode, to stdout or to a file

#### `generator`
The generator generates SWT sequences, sent through DIM to specified ALF IPbus instance. It supports:
- running multiple tests in sequence and running a test repeatedly, with specified wait time
- easy sequence creation, as an array of operations with `type` and `data` parameters
- performing specified operations on one or more registers or register blocks - as one or separate sequences for each register
- randomisation of operation order within sequences
- tracking mock's randomised responses and reviewing the test results accordingly
- logging in normal or verbose mode, to stdout or to a file

### Compilation
While in the `build` directory, run
```
cmake3 .. -DCMAKE_PREFIX_PATH=/opt/o2/lib
make
```

### Running the example
While still in the `build` directory, run
```
bin/mock -c ../example_config.toml
```
and then (with ALF IPbus running and configured for link 127.0.0.1:50001)
```
bin/generator -c ../example_config.toml
```
Full list of program options can be accessed with `-h`.
