CC = g++
CCFLAGS = -std=c++11 -g3 -ggdb -O0 -w -fno-rtti

LLCLANG = clang++ -stdlib=libc++
LDCONFIG = `llvm-config --ldflags --system-libs --libs`
LCCONFIG =  `llvm-config --cxxflags`
CLANGLIBS := \
        -Wl,--start-group \
        -lclangAST \
        -lclangASTMatchers \
        -lclangAnalysis \
        -lclangBasic \
        -lclangDriver \
        -lclangEdit \
        -lclangFrontend \
        -lclangFrontendTool \
        -lclangLex \
        -lclangParse \
        -lclangSema \
        -lclangEdit \
        -lclangRewrite \
        -lclangRewriteFrontend \
        -lclangStaticAnalyzerFrontend \
        -lclangStaticAnalyzerCheckers \
        -lclangStaticAnalyzerCore \
        -lclangSerialization \
        -lclangToolingCore \
        -lclangTooling \
        -lclangFormat \
        -Wl,--end-group

OBJS = rewriter.o

ALL: ${OBJS}
	${CC} ${OBJS} ${CLANGLIBS} ${LDCONFIG}  -o test

rewriter.o: rewriter.cpp
	${CC} ${CCFLAGS} ${LCCONFIG} -c rewriter.cpp

clean:
	rm test *.o
