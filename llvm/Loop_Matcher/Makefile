CC = g++
CCFLAGS = -std=c++11 -g -w -fno-rtti

LLCLANG = clang++ -stdlib=libc++
LDCONFIG = `llvm-config --ldflags --system-libs --libs`
LCCONFIG =  `llvm-config --cxxflags`
CLANGLIBS = \
  -L/home/moon/local/lib \
  -lclangFrontend \
  -lclangParse \
  -lclangSema \
  -lclangAnalysis \
  -lclangEdit\
  -lclangASTMatchers \
  -lclangAST \
  -lclangLex \
  -lclangDriver \
  -lclangSerialization \
  -lclangBasic \
  -lclangTooling \
  -lLLVMMC \
  -lLLVMSupport \

OBJS = main.o

ALL: ${OBJS}
	${CC} ${OBJS} ${CLANGLIBS} ${LDCONFIG}  -o test

main.o:	main.cpp
	${CC} ${CCFLAGS} ${LCCONFIG} -c main.cpp

clean:
	rm test *.o
