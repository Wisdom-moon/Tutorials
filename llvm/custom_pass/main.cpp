#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"

class MyCustomPass : public llvm::FunctionPass {
public:
  static char ID;
  MyCustomPass () : FunctionPass (ID) {}
  virtual bool runOnFunction (llvm::Function &F);
};

bool MyCustomPass::runOnFunction(llvm::Function &F) {
  llvm::errs() << "Hello: ";
  llvm::errs().write_escaped (F.getName()) << '\n';
  return false;
}

char MyCustomPass::ID = 0;
static llvm::RegisterPass<MyCustomPass> X("test_llvm", "test llvm", false, false);
