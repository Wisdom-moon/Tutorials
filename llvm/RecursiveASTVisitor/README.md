It is based on LLVM 5.0.1. 
It follows the tutorial "http://clang.llvm.org/docs/RAVFrontendAction.html"


It shows how to use clang API to traverse AST tree.
It is based on libTooling.

Usage:
./test "namespace n { namespace m { class C {}; } }"
