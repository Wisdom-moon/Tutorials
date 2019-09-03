#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/ADT/StringRef.h"

using namespace llvm;

void dumpFunctionCode(
  llvm::Function &F) {

  std::string fname = F.getName().str() + ".ir";

  std::string origName = fname;
  int counter = 0;
  while (pocl_exists (fname.c_str())) {
    std::ostringstream ss;
    ss << origName << "." << counter;
    fname = ss.str();
    ++counter;
  }

  StringRef S_name(fname);
  std::error_code EC;
  raw_fd_ostream S(S_name, EC, sys::fs::F_None);
  assert(!EC);
  F.print(S);
  S.close();
  std::cout << "### dumped Function " << F.getName().str() << " code to " << fname << std::endl;
}


int main ()
{

//Create module and builder.
  llvm::LLVMContext context;
  llvm::Module* module = new llvm::Module ("Top", context);
  llvm::IRBuilder<> builder (context);

//Create the declare statement of 'main' function.
  llvm::FunctionType *funcType = 
    llvm::FunctionType::get (builder.getVoidTy(), false);
  llvm::Function *mainFunc = 
    llvm::Function::Create (funcType, llvm::Function::ExternalLinkage, "main", module);

//Create the first basic block in 'main' function.
  llvm::BasicBlock *entry = llvm::BasicBlock::Create (context, "entrypoint", mainFunc);
  builder.SetInsertPoint (entry);

//Create the define statement of global string variable.
  llvm::Value *helloWorld = builder.CreateGlobalStringPtr ("hello world!\n", "Str");

//Declare another function 'puts'
  std::vector<llvm::Type *> putsArgs;
  putsArgs.push_back (builder.getInt8Ty()->getPointerTo());
  llvm::ArrayRef<llvm::Type *> argsRef(putsArgs);

  llvm::FunctionType *putsType = 
    llvm::FunctionType::get (builder.getInt32Ty(), argsRef, true);
  llvm::Constant *putsFunc = module->getOrInsertFunction ("printf", putsType);

//Call 'puts' in 'main'
  builder.CreateCall (putsFunc, helloWorld);
  builder.CreateRetVoid ();

  module->dump();
}
