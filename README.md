# AO Communication APP

This app communicates with the API of the Alpha Omega machine and provides

## Build

We are now using [meson](https://mesonbuild.com/) approach for setting up the build of this app.


### Caveat

This module is reworked currently. There is a short-coming with the sampling rate which is specified at 2000Hz, event though we usually have data streamed/incoming with 22_000Hz. The program broke for unknown reasons with a higher sampling rate specified. Note that this parameter will not impact the real sampling rate at which the data is incoming and streamed to LSL, but the LSL stream will have a wrong sampling rate shown for the nominal_srate.

#### The Dareplane Overview

[Back to the Dareplane landing page](https://github.com/bsdlab/Dareplane)


#### Trouble shooting

- App compiles but does not start and not print anything: This can be the case if you do not have the `lsl.dll` in the main directory! Just linking against the static library is not enough!
