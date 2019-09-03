It is based on LLVM 5.0.1 and libTooling.
It follows the tutorial "https://eli.thegreenplace.net/2014/05/01/modern-source-to-source-transformation-with-clang-and-libtooling/"


It shows how to use clang API to transform source code. 
This tool dumps the AST tree of the source code on the screen, and write modified code into file "Re_origin_file_name".

Usage:
./test hello.c -- -I/home/moon/local/lib/clang/5.0.1/include/
