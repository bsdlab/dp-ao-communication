# AO Communication APP

This app communicates with the API of the Alpha Omega machine and provides

## Build

Start with adjusting the `Makefile`, especially the following lines:

```Makefile

#### Includes
# Path to your python installation / Python.h file
INCS = C:\Users\bsdlab\anaconda3\include

# Dependencies from this project
INCS += ./src/include/libAO/include
INCS += ./src/include/liblsl/include

```

Also make sure that you have the `libAO` and `liblsl` libraries in the `src/include` directory. `libAO` is provided by Alpha Omega and [`liblsl`](https://github.com/sccn/liblsl) is just downloaded from the [LSL git repo](https://github.com/sccn/liblsl).

Building should then work with just using `make`.
