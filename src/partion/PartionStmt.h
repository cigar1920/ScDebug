#ifndef PartionSTMT_H
#define PartionSTMT_H

#include "../Headers.h"
#include "../Constants.h"
#include "../Utils.h"
set<int> underBlock;
class PartionStmtVisitor : public RecursiveASTVisitor<PartionStmtVisitor>
{
private:
	ASTContext *astContext; // used for getting additional AST info

	SourceLocation beginLoc;

	int varCount;

	bool isForInit;

	bool hasBinop;

	string sCom = "";

	int biExprCount;

	set<string> nonTerminalSet;

	int countBinaryOp(Stmt *stmt)
	{
		if (stmt == NULL)
			return 0;
		Stmt::child_iterator it = stmt->child_begin();
		int num = 0;
		for (; it != stmt->child_end(); it++)
		{
			num += countBinaryOp(*it);
		}
		if (isa<BinaryOperator>(*stmt))
		{
			num++;
		}
		return num;
	}
	VarInfo getVar(Expr *expr)
	{
		Expr *subExpr;
		if (BinaryOperator *specExpr = dyn_cast<BinaryOperator>(expr))
		{
			VarInfo varInfo = threeAddrCode(specExpr);
			varInfo.vars.push_back(varInfo.var);
			return varInfo;
		}
		else if (UnaryOperator *specExpr = dyn_cast<UnaryOperator>(expr))
		{
			string op = specExpr->getOpcodeStr(specExpr->getOpcode()).data();
			subExpr = specExpr->getSubExpr();
			VarInfo ret = getVar(subExpr);
			nonTerminalSet.insert(ret.var);
			return VarInfo("(" + op + "(" + ret.var + "))", ret.fpVar, ret.vars, true);
		}
		else if (ParenExpr *specExpr = dyn_cast<ParenExpr>(expr))
		{
			return getVar(specExpr->getSubExpr());
		}
		else if (ImplicitCastExpr *specExpr = dyn_cast<ImplicitCastExpr>(expr))
		{
			return getVar(specExpr->getSubExpr());
		}
		else if (CStyleCastExpr *specExpr = dyn_cast<CStyleCastExpr>(expr))
		{
			string type = specExpr->getType().getAsString().data();
			VarInfo ret = getVar(specExpr->getSubExpr());
			nonTerminalSet.insert(ret.var);
			return VarInfo("((" + type + ")" + ret.var + ")", ret.fpVar, ret.vars);
		}
		else if (ArraySubscriptExpr *specExpr = dyn_cast<ArraySubscriptExpr>(expr))
		{
			VarInfo ret = getVar(specExpr->getBase());
			VarInfo retInd = getVar(specExpr->getIdx());
			string ind = retInd.var;
			string arr = ret.var + "[" + ind + "]";
			retInd.vars.push_back(arr);
			nonTerminalSet.insert(arr);
			return VarInfo(arr, ret.fpVar, retInd.vars);
		}
		else if (MemberExpr *specExpr = dyn_cast<MemberExpr>(expr))
		{
			string base = getVar(specExpr->getBase()).var;
			bool isMynumber = false;
			specExpr->getBase()->getType().dump();
			if (strcmp(specExpr->getBase()->getType().getAsString().data(), "mynumber") == 0)
			{
				isMynumber = true;
			}
			string ref = specExpr->getMemberNameInfo().getAsString().data();
			if (specExpr->isArrow())
			{
				VarInfo ret(base + "->" + ref);
				if (isMynumber && ref.substr(0, 1) == "i")
				{
					ret.fpVar = base + "->d";
				}
				ret.vars.push_back(base + "->" + ref);
				nonTerminalSet.insert(base + "->" + ref);
				return ret;
			}
			else
			{
				VarInfo ret(base + "." + ref);
				if (isMynumber && ref.substr(0, 1) == "i")
				{
					ret.fpVar = base + ".d";
				}
				ret.vars.push_back(base + "." + ref);
				nonTerminalSet.insert(base + "." + ref);
				return ret;
			}
		}
		else
		{
			if (isa<ConditionalOperator>(*expr))
			{
				biExprCount += countBinaryOp(expr);
			}
			string ret = rewriter.getRewrittenText(getSourceRange(expr));
			cout << "------ret end-------------" << endl;
			cout << "ret:" << ret << endl;
			cout << "------ret end-------------" << endl;
			VarInfo varInfo(ret);
			nonTerminalSet.insert(ret);

			if (!isLiteral && !isMacro(expr))
				varInfo.vars.push_back(ret);
			return varInfo;
		}
	}
	VarInfo threeAddrCode(BinaryOperator *biExpr)
	{
		biExprCount++;
		Expr *lhs, *rhs;
		VarInfo varInfo("TempNull");

		string op = biExpr->getOpcodeStr().data();
		string type = biExpr->getType().getAsString();
		if (op == "=")
		{
			// SourceLocation endOfToken = clang::Lexer::findLocationAfterToken(
			// stmt->getLocEnd(), tok::r_brace, rewriter.getSourceMgr(), rewriter.getLangOpts(), false);
			// if(endOfToken.isValid())
			// partionFile << getColumn(endOfToken);
			// VarInfo leftVar = getVar(biExpr->getLHS(),true);
			string leftVarStr = rewriter.getRewrittenText(getSourceRange(biExpr->getLHS()));
			if (nonTerminalSet.find(leftVarStr) != nonTerminalSet.end())
			{
				underBlock.insert(atoi(getColumn(biExpr->getExprLoc()).c_str()));
				// ofstream partionFile("partion.log", ios::out | ios::app);
				// partionFile << getColumn(biExpr->getExprLoc());
				// partionFile << endl;
				nonTerminalSet.clear();
			}
			VarInfo rightVar = getVar(biExpr->getRHS());
		}
		else
		{
			VarInfo rightVar = getVar(biExpr->getRHS());
			VarInfo leftVar = getVar(biExpr->getLHS());
		}
		return varInfo;
	}
	string process(BinaryOperator *biExpr)
	{
		string texts("");
		cout << "iii " << biExprCount << endl;
		if (biExprCount == 0)
		{
			threeAddrCode(biExpr);
		}
		biExprCount--;
		return texts;
	}

public:
	explicit PartionStmtVisitor(CompilerInstance *CI)
		: astContext(&(CI->getASTContext())) // initialize private members
	{
		ofstream partionFile("partion.log");
		partionFile.close();
		varCount = 0;
		biExprCount = 0;
		isForInit = false;
		hasBinop = false;
		nonTerminalSet.clear();
	}
	virtual bool VisitBinaryOperator(BinaryOperator *biOp)
	{
		string type = biOp->getType().getAsString();
		// string type = biOp->getLHS()->getType().getAsString();
		if (type == "double" || type == "float" || type == "long double")
			process(biOp);
		return true;
	}
	virtual bool TraverseCompoundStmt(CompoundStmt *stmt)
	{
		if (!WalkUpFromCompoundStmt(stmt))
			return false;
		underBlock.insert(atoi(getColumn(stmt->getLocStart()).c_str()));
		// ofstream partionFile("partion.log", ios::out | ios::app);
		// partionFile << getColumn(stmt->getLocStart());
		// partionFile << endl;

		nonTerminalSet.clear();
		for (Stmt::child_range range = stmt->children(); range; ++range)
		{
			TraverseStmt(*range);
		}

		// SourceLocation endOfToken = clang::Lexer::findLocationAfterToken(
		// stmt->getLocEnd(), tok::r_brace, rewriter.getSourceMgr(), rewriter.getLangOpts(), false);
		// if(endOfToken.isValid())
		// partionFile << getColumn(endOfToken);
		// else

		// partionFile << getColumn(stmt->getLocEnd());
		underBlock.insert(atoi(getColumn(stmt->getLocEnd()).c_str()));
		// partionFile << endl;
		nonTerminalSet.clear();
		return true;
	}
	virtual bool VisitReturnStmt(ReturnStmt *stmt)
	{
		if (stmt->getRetValue() == NULL)
			return true;
		if (stmt->getRetValue()->getType().getAsString() != "float" && stmt->getRetValue()->getType().getAsString() != "double" && stmt->getRetValue()->getType().getAsString() != "long double")
			return true;
		if (!isSameFile(stmt->getRetValue()->getLocStart()) || !(isSameFile(stmt->getLocStart())))
			return true;
		SourceLocation loc = getPureLocFirst(stmt->getLocStart());
		if (SLcmp(loc, getPureLocFirst(stmt->getRetValue()->getLocStart())) != SL_LESS)
			return true;
		// rewriter.ReplaceText(getSourceRange(stmt->getRetValue()), info.varName);
		// string stackOP = "callExpStack.push(getReal(\"" + info.varName + "\",&" + info.varName + "));";
		// rewriter.InsertTextBefore(loc, info.decl + info.stmt + stackOP + "\n callExp++; /*identify the function is  add move tmpShadow*/\n");
		isChanged = true;
		underBlock.insert(atoi(getColumn(stmt->getLocStart()).c_str()));
		return true;
	}
};

class PartionStmtASTConsumer : public ASTConsumer
{
private:
	PartionStmtVisitor *visitor; // doesn't have to be private

public:
	// override the constructor in order to pass CI
	explicit PartionStmtASTConsumer(CompilerInstance *CI)
		: visitor(new PartionStmtVisitor(CI)) // initialize the visitor
	{
	}

	// override this to call our PartionStmtVisitor on the entire source file
	virtual void HandleTranslationUnit(ASTContext &Context)
	{
		/* we can use ASTContext to get the TranslationUnitDecl, which is
		a single Decl that collectively represents the entire source file */
		visitor->TraverseDecl(Context.getTranslationUnitDecl());
	}
};

class PartionStmtFrontendAction : public ASTFrontendAction
{
public:
	virtual std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef file)
	{
		rewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
		return std::unique_ptr<ASTConsumer>(new PartionStmtASTConsumer(&CI)); // pass CI pointer to ASTConsumer
	}
};

#endif
