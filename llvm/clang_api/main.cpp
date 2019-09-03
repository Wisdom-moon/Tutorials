#include "clang/Basic/TargetInfo.h"
#include "clang/AST/Decl.h"
#include "clang/AST/DeclGroup.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Lex/Token.h"
#include "clang/Parse/ParseAST.h"
#include "clang/Frontend/CompilerInstance.h"
#include <iostream>

using namespace clang;

class CustomASTConsumer : public ASTConsumer {
public:
  CustomASTConsumer () : ASTConsumer() {}
  virtual ~CustomASTConsumer () {}
  virtual bool HandleTopLevelDecl (DeclGroupRef decls)
  {
    clang::DeclGroupRef::iterator it;
    for (it = decls.begin(); it != decls.end(); it++)
    {
      clang::VarDecl *vd = llvm::dyn_cast<clang::VarDecl>(*it);
      if (vd)
	std::cout << vd->getDeclName().getAsString() << std::endl;
    }
    return true;
  }
};

int main ()
{
  CompilerInstance ci;
  ci.createDiagnostics ();
  const std::shared_ptr<TargetOptions> p_to(new TargetOptions ());
  p_to->Triple = llvm::sys::getDefaultTargetTriple ();
  TargetInfo *pti = TargetInfo::CreateTargetInfo (ci.getDiagnostics(), p_to);
  ci.setTarget (pti);
  ci.createFileManager ();
  ci.createSourceManager (ci.getFileManager ());
  ci.createPreprocessor (TU_Complete);

  ci.createASTContext();
  CustomASTConsumer *astConsumer = new CustomASTConsumer ();
  std::unique_ptr<ASTConsumer> p_astConsumer(std::move(astConsumer));
  ci.setASTConsumer (std::move(p_astConsumer));

  const FileEntry *pFile = ci.getFileManager().getFile ("hello.c");
  FileID ID_file = ci.getSourceManager().getOrCreateFileID (pFile, SrcMgr::C_User);
  ci.getSourceManager().setMainFileID(ID_file);
  //ci.getPreprocessor().EnterMainSourceFile ();
  ci.getDiagnosticClient().BeginSourceFile (ci.getLangOpts(), &ci.getPreprocessor());

  clang::ParseAST (ci.getPreprocessor(), &(ci.getASTConsumer()), ci.getASTContext());
/*
  Token tok;
  do {
    ci.getPreprocessor().Lex (tok);
    if (ci.getDiagnostics().hasErrorOccurred ())
      break;
    ci.getPreprocessor().DumpToken (tok);
    std::cerr << std::endl;
  } while (tok.isNot(clang::tok::eof));
*/
  ci.getDiagnosticClient().EndSourceFile ();
  return 0;
}
