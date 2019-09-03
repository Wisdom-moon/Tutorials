CC = g++
CCFLAGS = -std=c++11 -g -w

LLCLANG = clang++ -stdlib=libc++
LDCONFIG = -g `llvm-config --ldflags --system-libs --libs core`
LCCONFIG = -g `llvm-config --cxxflags`

OBJS = main.o

ALL: ${OBJS}
	${CC} ${OBJS} ${LDCONFIG}  -o test

main.o:	main.cpp
	${CC} ${CCFLAGS} ${LCCONFIG} -c main.cpp

clean:
	rm test *.o
