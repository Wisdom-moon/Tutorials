CC = g++
CCFLAGS = -std=c++11 -g -w

LLCLANG = clang++ -stdlib=libc++
LDCONFIG = `llvm-config --ldflags --system-libs`
LCCONFIG =  `llvm-config --cxxflags`

OBJS = main.o

ALL: ${OBJS}
	${CC} -shared ${OBJS} ${LDCONFIG}  -o mycustom_pass.so

main.o:	main.cpp
	${CC} ${CCFLAGS} ${LCCONFIG} -c main.cpp

clean:
	rm *.so *.o
