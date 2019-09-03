It is based on LLVM 5.0.1. 
It follows the tutorial "https://www.ibm.com/developerworks/library/os-createcompilerllvm2/index.html"


It shows how to write a llvm function pass.
Usage:
  opt -load ./mycustom_pass.so -test_llvm < hello.bc > /dev/null

Warning:
  You should not pass --libs to llvm-config. It cause conflict with symbols coming from opt and your pass.
