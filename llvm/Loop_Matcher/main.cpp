#include "clang/AST/ASTContext.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "llvm/Support/CommandLine.h"

using namespace clang;
using namespace clang::tooling;
using namespace clang::ast_matchers;
using namespace llvm;

StatementMatcher LoopMatcher = 
  forStmt(hasLoopInit(declStmt(
	    hasSingleDecl(varDecl(hasInitializer(integerLiteral(equals(0))))
	      .bind("initVarName")))),
	  hasIncrement(unaryOperator(
	    hasOperatorName("++"),
	    hasUnaryOperand(declRefExpr(
	      to(varDecl(hasType(isInteger())).bind("incVarName")))))),
	  hasCondition(binaryOperator(
	    hasOperatorName("<"),
	    hasLHS(ignoringParenImpCasts(declRefExpr(
	      to(varDecl(hasType(isInteger())).bind("condVarName"))))),
	    hasRHS(expr(hasType(isInteger())))))).bind("forLoop");


static bool areSameVariable(const ValueDecl *First, const ValueDecl *Second) {
  return First && Second &&
	 First->getCanonicalDecl() == Second->getCanonicalDecl();
}

class LoopPrinter : public MatchFinder::MatchCallback {
public :
  virtual void run(const MatchFinder::MatchResult &Result);
};

void LoopPrinter::run(const MatchFinder::MatchResult &Result) {
  ASTContext *Context = Result.Context;
  const ForStmt *FS = Result.Nodes.getNodeAs<ForStmt>("forLoop");

  if (!FS ||!Context->getSourceManager().isWrittenInMainFile(FS->getForLoc()))
    return;
  const VarDecl *IncVar = Result.Nodes.getNodeAs<VarDecl>("incVarName");
  const VarDecl *CondVar = Result.Nodes.getNodeAs<VarDecl>("condVarName");
  const VarDecl *InitVar = Result.Nodes.getNodeAs<VarDecl>("initVarName");

  if (!areSameVariable(IncVar, CondVar) || !areSameVariable(IncVar, InitVar))
    return;

  llvm::outs() << "Potential array-based loop discovered.\n";
}

static llvm::cl::OptionCategory MyToolCategory("My-tool options");

static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

static cl::extrahelp MoreHelp("\nMore help text...");

int main (int argc, const char **argv) {
  CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
  ClangTool Tool(OptionsParser.getCompilations(),
		OptionsParser.getSourcePathList());

  LoopPrinter Printer;
  MatchFinder Finder;
  Finder.addMatcher (LoopMatcher, &Printer);

  return Tool.run(newFrontendActionFactory(&Finder).get());
}
