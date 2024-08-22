# ALF IPbus tester
This repository contains the code which will be used to thoroughly test [ALF IPbus](https://github.com/VictorPierozak/ALFIPbus), a temporary solution for migrating the FIT Detector Control System to the standard ALFRED framework developed for ALICE, CERN.

## Compilation
While in the `build` directory, run
```
cmake3 .. -DCMAKE_PREFIX_PATH=/opt/o2/lib
make
```

## Running
While still in the `build` directory, run
```
bin/generator
```
or
```
bin/mock
```