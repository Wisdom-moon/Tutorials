//OMP Paralle For AST
    Stmt::StmtClass sc = s->getStmtClass();
    if (sc == clang::Stmt::OMPParallelForDirectiveClass) {
      const Stmt *Body = ((OMPForDirective *)s)->getAssociatedStmt();
      //This is the For stmt captured by OMP Paralle FOR
      Stmt * b = ((CapturedStmt *)Body)->getCapturedStmt();
      //This is the outlined function declaration.
      CapturedDecl *decl = ((CapturedStmt *)Body)->getCapturedDecl();
