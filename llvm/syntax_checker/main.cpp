#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"

using namespace clang::tooling;
using namespace llvm;

static llvm::cl::OptionCategory MyToolCategory("My-tool options");

static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

static cl::extrahelp MoreHelp("\nMore help text...");

int main (int argc, const char **argv) {
  CommonOptionsParser OptionsParser (argc, argv, MyToolCategory);
  ClangTool Tool(OptionsParser.getCompilations(),
		OptionsParser.getSourcePathList());

  return Tool.run(newFrontendActionFactory<clang::SyntaxOnlyAction>().get());
}
