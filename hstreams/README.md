The step to run hStreams applications.
0. Make sure the mpss daemon is running.
1. export the environment variables from /opt/intel/compilers_and_libraries.
2. make
3. run the run.sh

Tips:
1. if the type of kernel arg  is float, it must be directly converted to uint64, otherwise will get the wrong value.
2. In multistream, the data transfer from device to host, will also need transfer on the same streams as mem xfer from host to dev and kernel execution?
   if streams_per_place = 2 or more, places_per_domain = 1.
