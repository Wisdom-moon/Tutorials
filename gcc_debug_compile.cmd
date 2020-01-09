../gcc-ocl/configure --prefix=$(pwd)/../gcc-install --enable-languages=c,c++,ocl --disable-multilib --with-gmp=/home/moon/local --with-mpfr=/home/moon/local --with-mpc=/home/moon/local CFLAGS=" -O0 -g3 -ggdb" CPPFLAGS=" -O0 -g3 -ggdb" CXXFLAGS=" -O0 -g3 -ggdb" CFLAGS_FOR_BUILD=" -O0 -g3 -ggdb" CXXFLAGS_FOR_BUILD=" -O0 -g3 -ggdb" CXXFLAGS_FOR_TARGET=" -O0 -g3 -ggdb" CFLAGS_FOR_TARGET=" -O0 -g3 -ggdb" BOOT_CFLAGS=" -O0 -g3 -ggdb"

gcc -c test.cpp -wrapper gdb,--args
gcc -c test.cpp -fdump-tree-original : dump the GENERIC
gfortran -c test.cpp -fdump-parse-tree: dump the gfc_code tree
gcc -c test.cpp -fdump-tree-all: dump all the tree
