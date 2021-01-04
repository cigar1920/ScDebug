#ifndef INSERTPAREN_H
#define INSERTPAREN_H

#include "../Headers.h"
#include "../Constants.h"
#include "../Utils.h"

class InsertParenVisitor : public RecursiveASTVisitor<InsertParenVisitor> {
private:
	ASTContext *astContext; // used for getting additional AST info

	SourceLocation beginLoc;

	//insert {} to the bare stmt
	void insert(Stmt * stmt) {
		if (stmt == NULL)
			return;
		if (isa<CompoundStmt>(stmt))
			return;
		if (!isSameFile(stmt->getLocStart()))
			return;
		SourceLocation endOfToken = clang::Lexer::findLocationAfterToken(
        stmt->getLocEnd(), tok::semi, rewriter.getSourceMgr(), rewriter.getLangOpts(), false);
		rewriter.InsertTextBefore(stmt->getLocStart(), "{\n");
		if (!endOfToken.isValid()) { // The end of stmt is "}", so there is no ";" after "}"
			endOfToken = stmt->getLocEnd();
		}
		rewriter.InsertTextAfterToken(endOfToken, "\n}\n");
		isChanged = true;
	}

public:
	explicit InsertParenVisitor(CompilerInstance *CI) 
		:astContext(&(CI->getASTContext())) // initialize private members
	{
		//rewriter.setSourceMgr(astContext->getSourceManager(), astContext->getLangOpts());
	}

	virtual bool VisitForStmt(ForStmt * stmt) {
		insert(stmt->getBody());
		return true;
	}

	virtual bool VisitIfStmt(IfStmt * ifStmt) {
		cout << "then" << endl;
		if (ifStmt->getThen() != NULL) ifStmt->getThen()->dump();
		cout << "else" << endl;
		if (ifStmt->getElse() != NULL) ifStmt->getElse()->dump();
		insert(ifStmt->getThen());
		insert(ifStmt->getElse());
		return true;
	}

	virtual bool VisitWhileStmt(WhileStmt * stmt) {
		insert(stmt->getBody());
		return true;
	}

	virtual bool VisitDoStmt(DoStmt * stmt) {
		insert(stmt->getBody());
		return true;
	}

	virtual bool VisitCaseStmt(CaseStmt * stmt) {
		insert(stmt->getSubStmt());
		return true;
	}

	virtual bool VisitLabelStmt(LabelStmt * stmt) {
		insert(stmt->getSubStmt());
		return true;
	}
	
	virtual bool VisitDefaultStmt(DefaultStmt * stmt) {
		insert(stmt->getSubStmt());
		return true;
	}

};



class InsertParenASTConsumer : public ASTConsumer {
private:
	InsertParenVisitor *visitor; // doesn't have to be private

public:
	// override the constructor in order to pass CI
	explicit InsertParenASTConsumer(CompilerInstance *CI)
		: visitor(new InsertParenVisitor(CI)) // initialize the visitor
	{ }

	// override this to call our InsertParenVisitor on the entire source file
	virtual void HandleTranslationUnit(ASTContext &Context) {
		/* we can use ASTContext to get the TranslationUnitDecl, which is
		a single Decl that collectively represents the entire source file */
		visitor->TraverseDecl(Context.getTranslationUnitDecl());
	}
};

class InsertParenFrontendAction : public ASTFrontendAction {
public:
	virtual std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef file) {
		rewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
		return std::unique_ptr<ASTConsumer>(new InsertParenASTConsumer(&CI)); // pass CI pointer to ASTConsumer
	}
};

#endif