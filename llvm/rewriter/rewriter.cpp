//------------------------------------------------------------------------------
// Tooling sample. Demonstrates:
//
// * How to write a simple source tool using libTooling.
// * How to use RecursiveASTVisitor to find interesting AST nodes.
// * How to use the Rewriter API to rewrite the source code.
//
// Peng Zhang(pengzhang_nudt@sina.com)
// This code is in the public domain
//------------------------------------------------------------------------------
#include <sstream>
#include <string>
#include <iostream>

#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/Type.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/APSInt.h"

using namespace clang;
using namespace clang::driver;
using namespace clang::tooling;


ASTContext *Ctx;
//How It known which options are include in this Category???
static llvm::cl::OptionCategory ToolingSampleCategory("Tooling Sample");

//The RecursiveASTVisitor class does preordor or postorder depth-first
//traversal on the entire Clang AST and visits each node.
//It can perform three distinct tasks:
//  1. traverse the AST;---TraverseStmt/TraverseDecl/Traverse*
//  2. at a given node, walk up the class hierarchy;---WalkUpFrom*
//  3. given a (node, class) combination, call a user-overridable function
//  to actually visit the node.---Visit*
//The * can be declarations, types, statements, expressions or other AST nodes.

// By implementing RecursiveASTVisitor, we can specify which AST nodes
// we're interested in by overriding relevant methods.
class MyASTVisitor : public RecursiveASTVisitor<MyASTVisitor> {
public:
  MyASTVisitor(Rewriter &R) : TheRewriter(R) { tmp = NULL;}

  //Invoked before visiting a statement or expression.
  //Return false to skip visiting the node.
  bool dataTraverseStmtPre (Stmt *S) {
    return true;
  }

  //Invoked after visiting a statement or expression via data recursion.
  //return false if the visitation was terminated early.
  bool dataTraverseStmtPost (Stmt *S) {
    return true;
  }

//If return false, it will stop the total traverse.
//Stmt include : 
//ImplicitCastExpr,ConditionalOperator and so on, not just *Stmt.
  bool VisitStmt(Stmt *s) {
    // Only care about If statements.
    std::cerr <<"************VisitStmt DUMP***********\n";
    s->dump();
    if (isa<IfStmt>(s)) {
      IfStmt *IfStatement = cast<IfStmt>(s);
      Stmt *Then = IfStatement->getThen();

      TheRewriter.InsertText(Then->getLocStart(), "// the 'if' part\n", true,
                             true);

      Stmt *Else = IfStatement->getElse();
      if (Else)
        TheRewriter.InsertText(Else->getLocStart(), "// the 'else' part\n",
                               true, true);
    }

    return true;
  }

  bool VisitDeclRefExpr (DeclRefExpr * e) {

    std::cerr <<"************VisitDeclRefExpr DUMP***********\n";
    e->dump();  

//Create an IntegerLiteral.
    //APInt(unsigned unmBits, uint64_t val, bool isSigned)
   // llvm::APInt int_val(64, 0, true);
    if (tmp)
	tmp->dump();
    else {
      llvm::APInt int_val(64, 8, true);
      //llvm::APInt *int_val = new (*Ctx) llvm::APInt(64, 0, true);
      QualType type = Ctx->getIntTypeForBitwidth(64, true);
      IntegerLiteral * int_expr = new (*Ctx) IntegerLiteral(*Ctx, int_val, type, e->getLocation());
      //IntegerLiteral * int_expr = new (*Ctx) IntegerLiteral(*Ctx, *int_val, type, e->getLocation());
      std::cerr <<"************IntegerLiteral DUMP***********\n";
      int_expr->dump();  
      tmp = int_expr;
    }
    
    return true;
  }

  //ParmVarDecl is child class of VarDecl, and its type is DECL::ParmVar.
  //But for a ParmVarDecl, it will first call VisitVarDecl to visit this decl, 
  //Then call VisitParmVarDecl visit it too!
  bool VisitVarDecl(VarDecl *d) {
    return true;
  }

  bool VisitParmVarDecl(ParmVarDecl *d) {
    return true;
  }


  bool VisitFunctionDecl(FunctionDecl *f) {
    // Only function definitions (with bodies), not declarations.
    if (f->hasBody()) {
      Stmt *FuncBody = f->getBody();

      // Type name as string
      QualType QT = f->getReturnType();
      std::string TypeStr = QT.getAsString();

      // Function name
      DeclarationName DeclName = f->getNameInfo().getName();
      std::string FuncName = DeclName.getAsString();

      // Add comment before
      std::stringstream SSBefore;
      SSBefore << "// Begin function " << FuncName << " returning " << TypeStr
               << "\n";
      SourceLocation ST = f->getSourceRange().getBegin();
      TheRewriter.InsertText(ST, SSBefore.str(), true, true);

      // And after
      std::stringstream SSAfter;
      SSAfter << "\n// End function " << FuncName;
      ST = FuncBody->getLocEnd().getLocWithOffset(1);
      TheRewriter.InsertText(ST, SSAfter.str(), true, true);
    }

    return true;
  }

/*
  //Dispatch Stmt to Traverse###Stmt for each concrete ###Stmt in StmtNodes.inc
  //dataTraverseNote is private function, can not be used in this function.
  bool dataTraverseNode (Stmt *S, DataRecursionQueue *Queue) {
    if (BinaryOperator *BinOp = dyn_cast<BinaryOperator>(S)) {
      std::cerr <<"*********dataTraverseNode BinaryOperator DUMP*********\n";
      S->dump();
      return true;
    } else if (UnaryOperator *UnOp = dyn_cast<UnaryOperator>(S)) {
      std::cerr <<"*********dataTraverseNode UnaryOperator DUMP*********\n";
      S->dump();
      return true;
    }
    return RecursiveASTVisitor<MyASTVisitor>::dataTraverseNode(S, Queue);
  }
*/

/*
  //TraverseBinaryOperator is not called by Clang...
  //Because in dataTraverseNode function, 
  //it call TraverseBin##Operator function instead.
  bool TraverseBinaryOperator(BinaryOperator*S) {
    bool ShouldVisitChildren = true;                                           
    bool ReturnValue = true;                                                   
    //Visit S
    std::cerr <<"************TraverseBinaryOperator DUMP***********\n";
    S->dump();  
  
    if (ShouldVisitChildren) {                                                 
      //for (Stmt *SubStmt : S->children()) {                                    
      //  TRY_TO_TRAVERSE_OR_ENQUEUE_STMT(SubStmt);                    
      //}                                                                        
        RecursiveASTVisitor<MyASTVisitor>::TraverseStmt(S->getRHS());//Omit lhs.
    }                                                                          
    return ReturnValue;  
  }
*/
  bool TraverseBinAssign(BinaryOperator*S) {
    bool ShouldVisitChildren = true;                                           
    bool ReturnValue = true;                                                   
    //Visit S
    std::cerr <<"************TraverseBinAssign DUMP***********\n";
    S->dump();  
  
    if (ShouldVisitChildren) {                                                 
      //for (Stmt *SubStmt : S->children()) {                                    
      //  TRY_TO_TRAVERSE_OR_ENQUEUE_STMT(SubStmt);                    
      //}                                                                        
        RecursiveASTVisitor<MyASTVisitor>::TraverseStmt(S->getRHS());//Omit lhs.
    }                                                                          
    return ReturnValue;  
  }

  bool TraverseStmt (Stmt *S) {
    if (!S)
      return true;

    if (BinaryOperator *BinOp = dyn_cast<BinaryOperator>(S)) {
      if (BinOp->getOpcode() == BO_Assign) {
        std::cerr <<"************TraverseStmt BO_Assign DUMP***********\n";
        S->dump();  
        return RecursiveASTVisitor<MyASTVisitor>::TraverseStmt(BinOp->getRHS());
      }
    }
    return RecursiveASTVisitor<MyASTVisitor>::TraverseStmt(S);
  }


private:
//Rewriter is the main interface to the rewrite buffers.
//Its primary job is to dispatch high-level requests to the
//low-level RewriteBuffers that is involved.
  Rewriter &TheRewriter;
  Expr * tmp;
};

//ASTConsumer is an abstract interface that should be implemented by clients that read ASTs.
//This abstraction layer allows the client to be independent of the AST producer(parser, AST dump file reader).
// Implementation of the ASTConsumer interface for reading an AST produced
// by the Clang parser.
class MyASTConsumer : public ASTConsumer {
public:
  MyASTConsumer(Rewriter &R) : Visitor(R) {}

  // Override the method that gets called for each parsed top-level
  // declaration.
  bool HandleTopLevelDecl(DeclGroupRef DR) override {
    for (DeclGroupRef::iterator b = DR.begin(), e = DR.end(); b != e; ++b) {
      // Traverse the declaration using our AST visitor.
      if (Ctx->getSourceManager().isWrittenInMainFile((*b)->getLocation())) {
        (*b)->dump();
        Visitor.TraverseDecl(*b);
      }
    }
    return true;
  }

//This is called to initialize the consumer, providing the ASTContext.
  virtual void Initialize(ASTContext &Context) {
    Ctx = &Context;
  }

private:
  MyASTVisitor Visitor;
};


//ASTFrontendAction is the abstract base class to use for AST consumer-based frontend actions.
// For each source file provided to the tool, a new FrontendAction is created.
class MyFrontendAction : public ASTFrontendAction {
public:
  MyFrontendAction() {}

//This routine is called as part of BeginSourceFile().
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                 StringRef file) override {
    llvm::errs() << "** Creating AST consumer for: " << file << "\n";
    TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
    return llvm::make_unique<MyASTConsumer>(TheRewriter);
  }

//BeginInvocation(): Callback before starting processing a single input,
//		giving the opportunity to modify the CompilerInvocation or do some
//		other action before BeginSourceFileAction is called.
//BeginSourceFileAction(): Callback at the start of processing a single input.
//ExecuteAction(): Callback to run the program action. This is guaranteed to only be
//		called between BeginSourceFileAction() and EndSourceFileAction().
  
//Callback at the end of processing a single input,
//to determin if the output files should be erased or not.
  void EndSourceFileAction() override {
    SourceManager &SM = TheRewriter.getSourceMgr();
    llvm::errs() << "** EndSourceFileAction for: "
                 << SM.getFileEntryForID(SM.getMainFileID())->getName() << "\n";

    // Now emit the rewritten buffer.
    //TheRewriter.getEditBuffer(SM.getMainFileID()).write(llvm::outs());
    std::string f_name = SM.getFileEntryForID(SM.getMainFileID())->getName().str();
    int f_index = f_name.find_last_of('/');
    f_name = f_name.substr(f_index+1,-1);
    std::stringstream out_name;
    out_name << "Re_" << f_name;
    std::error_code EC;
    llvm::raw_fd_ostream out_s(out_name.str(), EC, llvm::sys::fs::F_Text);
    TheRewriter.getEditBuffer(SM.getMainFileID()).write(out_s);
  }


private:
//The primary job of Rewriter is to dispatch high-level requests to the low-level RewriteBuffers.
  Rewriter TheRewriter;
};

int main(int argc, const char **argv) {

//It parses command-line, initializes a compilation database.
//It only retrieve options what belong to ToolingSampleCategory.
//The compilation database is used to supply information of a build, 
//such as compile commands "clang++ -o xx xxx.cc";
  CommonOptionsParser op(argc, argv, ToolingSampleCategory);

//op.getCompilations() return a reference to the loaded compilations database.
//op.getSourcePathList() returns a list of source file paths to process.
//Clang Tool is initialized with a CompilationDatabase and a set of files to run over.
  ClangTool Tool(op.getCompilations(), op.getSourcePathList());

  // ClangTool::run accepts a FrontendActionFactory, which is then used to
  // create new objects implementing the FrontendAction interface. Here we use
  // the helper newFrontendActionFactory to create a default factory that will
  // return a new MyFrontendAction object every time.
  // To further customize this, we could create our own factory class.
  return Tool.run(newFrontendActionFactory<MyFrontendAction>().get());
}
