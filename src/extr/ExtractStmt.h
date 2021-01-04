#ifndef EXTRACTSTMT_H
#define EXTRACTSTMT_H

#include "../Headers.h"
#include "../Constants.h"
#include "../Utils.h"

class ExtractStmtVisitor : public RecursiveASTVisitor<ExtractStmtVisitor>
{
private:
	ASTContext *astContext; // used for getting additional AST info

	SourceLocation beginLoc;

	int varCount;

	bool isForInit;

	bool hasBinop;

	string sCom = "";

	string varStr = "";

	CondInfo condInfoProducer(Expr *cond)
	{
		CondInfo info;
		if (cond != NULL)
		{
			cond->dump();
			string type = cond->getType().getAsString().data();
			string varName = extractVar + toString(varCount);
			varCount++;
			info.decl.append(type)
				.append(" ")
				.append(varName)
				.append(";\n");
			info.stmt.append(varName)
				.append(" = ")
				.append(rewriter.getRewrittenText(getSourceRange(cond)))
				.append(";\n");
			info.varName = varName;
		}
		return info;
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
			varStr.append("(").append(ret.var).append(")");
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
			varStr.append("(").append(ret.var).append(")");
			return VarInfo("((" + type + ")" + ret.var + ")", ret.fpVar, ret.vars);
		}
		else if (ArraySubscriptExpr *specExpr = dyn_cast<ArraySubscriptExpr>(expr))
		{
			VarInfo ret = getVar(specExpr->getBase());
			VarInfo retInd = getVar(specExpr->getIdx());
			string ind = retInd.var;
			string arr = ret.var + "[" + ind + "]";
			retInd.vars.push_back(arr);
			varStr.append("(").append(arr).append(")");
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
				varStr.append("(").append(base).append(")");
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
				varStr.append("(").append(base).append(")");
				return ret;
			}
		}
		else
		{

			string ret = rewriter.getRewrittenText(getSourceRange(expr));
			cout << "------ret end-------------" << endl;
			cout << "ret:" << ret << endl;
			cout << "------ret end-------------" << endl;
			VarInfo varInfo(ret);
			varStr.append("(").append(ret).append(")");
			varInfo.isConst = true; // record the const
			if (!isLiteral && !isMacro(expr))
				varInfo.vars.push_back(ret);
			return varInfo;
		}
	}
	VarInfo threeAddrCode(BinaryOperator *biExpr)
	{
		Expr *lhs, *rhs;
		VarInfo varInfo("TempNull");

		VarInfo rightVar = getVar(biExpr->getRHS());
		string op = biExpr->getOpcodeStr().data();
		string type = biExpr->getType().getAsString();
		if (op == "=")
		{
			if (varStr != "")
			{
				string texts = "//";
				string leftVarStr = rewriter.getRewrittenText(getSourceRange(biExpr->getLHS()));
				texts.append(leftVarStr).append("\n/*");
				texts.append(varStr).append("*/\n");
				if (containPara(leftVarStr, varStr))
				{
					// string texts = "";
					string varName = extractVar + toString(varCount);
					varCount++;
					rewriter.ReplaceText(getSourceRange(biExpr->getLHS()), type + " " + varName + "; " + varName);
					texts.append(leftVarStr).append(" = ").append(varName).append(";");
					SourceLocation endOfStmt = clang::Lexer::findLocationAfterToken(biExpr->getLocEnd(),
																					tok::semi, rewriter.getSourceMgr(), rewriter.getLangOpts(), false);
					if (endOfStmt.isValid())
					{
						rewriter.InsertTextAfter(endOfStmt, texts); //biExpr->getLocEnd() , texts);
						cout << "- end -" << endOfStmt.printToString(rewriter.getSourceMgr()) << endl;
					}
					varStr = "";
				}
			}
			varStr = "";
		}
		else
			VarInfo leftVar = getVar(biExpr->getLHS());
		return varInfo;
	}

public:
	explicit ExtractStmtVisitor(CompilerInstance *CI)
		: astContext(&(CI->getASTContext())) // initialize private members
	{
		varCount = 0;
		isForInit = false;
		hasBinop = false;
	}

	virtual bool VisitBinaryOperator(BinaryOperator *biOp)
	{
		string type = biOp->getType().getAsString();
		// string type = biOp->getLHS()->getType().getAsString();
		if (type == "double" || type == "float" || type == "long double")
			threeAddrCode(biOp);
		return true;
	}

	// virtual bool VisitIfStmt(IfStmt *stmt)
	// {
	// 	if (stmt->getCond() == NULL)
	// 		return true;
	// 	if (!isSameFile(stmt->getCond()->getLocStart()) || !(isSameFile(stmt->getLocStart())))
	// 		return true;
	// 	SourceLocation loc = getPureLocFirst(stmt->getLocStart());
	// 	if (SLcmp(loc, getPureLocFirst(stmt->getCond()->getLocStart())) != SL_LESS)
	// 		return true;
	// 	CondInfo info = condInfoProducer(stmt->getCond());
	// 	rewriter.ReplaceText(getSourceRange(stmt->getCond()), info.varName);
	// 	rewriter.InsertTextBefore(getPureLocFirst(stmt->getLocStart()), info.decl + info.stmt);
	// 	isChanged = true;
	// 	return true;
	// }

	virtual bool VisitSwitchStmt(SwitchStmt *stmt)
	{
		if (stmt->getCond() == NULL)
			return true;
		if (!isSameFile(stmt->getCond()->getLocStart()) || !(isSameFile(stmt->getLocStart())))
			return true;
		SourceLocation loc = getPureLocFirst(stmt->getLocStart());
		if (SLcmp(loc, getPureLocFirst(stmt->getCond()->getLocStart())) != SL_LESS)
			return true;
		CondInfo info = condInfoProducer(stmt->getCond());
		rewriter.ReplaceText(getSourceRange(stmt->getCond()), info.varName);
		rewriter.InsertTextBefore(getPureLocFirst(stmt->getLocStart()), info.decl + info.stmt);
		isChanged = true;
		return true;
	}

	virtual bool VisitWhileStmt(WhileStmt *stmt)
	{
		if (stmt->getCond() == NULL)
			return true;
		if (!isSameFile(stmt->getCond()->getLocStart()) || !(isSameFile(stmt->getLocStart())) || !(isSameFile(stmt->getBody()->getLocStart())))
			return true;
		SourceLocation loc1 = getPureLocFirst(stmt->getLocStart());
		SourceLocation loc2 = getPureLocSecond(stmt->getBody()->getLocEnd());
		SourceLocation loc3 = getPureLocFirst(stmt->getCond()->getLocStart());
		if (!(SLcmp(loc1, loc3) == SL_LESS && SLcmp(loc3, loc2) == SL_LESS))
			return true;
		CondInfo info = condInfoProducer(stmt->getCond());
		rewriter.ReplaceText(getSourceRange(stmt->getCond()), info.varName);
		rewriter.InsertTextBefore(loc1, info.decl + info.stmt);
		rewriter.InsertTextBefore(loc2, info.stmt);
		isChanged = true;
		return true;
	}

	virtual bool VisitDoStmt(DoStmt *stmt)
	{
		if (stmt->getCond() == NULL)
			return true;
		if (!isSameFile(stmt->getCond()->getLocStart()) || !(isSameFile(stmt->getLocStart())) || !(isSameFile(stmt->getBody()->getLocStart())))
			return true;
		SourceLocation loc1 = getPureLocSecond(stmt->getBody()->getLocEnd());
		SourceLocation loc2 = getPureLocFirst(stmt->getLocStart());
		if (!(SLcmp(loc2, loc1) == SL_LESS && SLcmp(loc1, getPureLocFirst(stmt->getCond()->getLocStart())) == SL_LESS))
			return true;
		CondInfo info = condInfoProducer(stmt->getCond());
		rewriter.ReplaceText(getSourceRange(stmt->getCond()), info.varName);
		rewriter.InsertTextBefore(loc1, info.stmt);
		rewriter.InsertTextBefore(loc2, info.decl);
		isChanged = true;
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
		CondInfo info = condInfoProducer(stmt->getRetValue());
		rewriter.ReplaceText(getSourceRange(stmt->getRetValue()), info.varName);
		string stackOP = "callExpStack.push(getReal(\"" + info.varName + "\",(Addr)&" + info.varName + "));";
		rewriter.InsertTextBefore(loc, info.decl + info.stmt + stackOP + "\n callExp++; /*identify the function is  add move tmpShadow*/\n");
		isChanged = true;
		return true;
	}
	virtual bool VisitUnaryOperator(UnaryOperator *unOp)
	{
		unOp->dump();
		string type = unOp->getType().getAsString();
		string text("(");
		string var = rewriter.getRewrittenText(getSourceRange(*(unOp->children())));
		if (type == "double" || type == "float" || type == "long double")
			if (unOp->getOpcode() == 0)
			{

				text = "SC_DEBUG_POSTINC((Addr)&" + var + " , " + var + ")";
				rewriter.ReplaceText(getSourceRange(unOp), text);
			}
			else if (unOp->getOpcode() == 1)
			{

				text = "SC_DEBUG_POSTFIX((Addr)&" + var + " , " + var + ")";
				rewriter.ReplaceText(getSourceRange(unOp), text);
			}
			else if (unOp->getOpcode() == 2)
			{

				text = "SC_DEBUG_PREINC((Addr)&" + var + " , " + var + ")";
				rewriter.ReplaceText(getSourceRange(unOp), text);
			}
			else if (unOp->getOpcode() == 3)
			{

				text = "SC_DEBUG_PREFIX((Addr)&" + var + " , " + var + ")";
				rewriter.ReplaceText(getSourceRange(unOp), text);
			}
			else if (unOp->getOpcode() == 7)
			{
				text = "(0.0 - " + var +")";
				rewriter.ReplaceText(getSourceRange(unOp), text);
			}

		for (Stmt::child_range range = unOp->children(); range; ++range)
		{
			cout << "unOp :" << rewriter.getRewrittenText(getSourceRange(*range)) << endl
				 << unOp->getOpcode() << endl;
		}
		// if (type == "double" || type == "float" || type == "long double")
		// {
		// cout<<"unOp "<<type<<" "<< getOpcodeStr(unOp).data()<<endl;
		// }
		return true;
	}

	virtual bool TraverseForStmt(ForStmt *stmt)
	{
		if (!WalkUpFromForStmt(stmt))
			return false;
		for (Stmt::child_range range = stmt->children(); range; ++range)
		{
			if (stmt->getInit() != NULL && *range == stmt->getInit())
				isForInit = true;
			else
				isForInit = false;
			TraverseStmt(*range);
		}
		isForInit = false;
		return true;
	}

	// virtual bool VisitVarDecl(VarDecl * decl) {
	// 	if (decl->getInit() == NULL || isForInit)
	// 		return true;
	// 	decl->dump();
	// 	if(1){
	// 		string name = decl->getNameAsString();
	// 		string type = decl->getType().getAsString().data();
	// 		try{
	// 			BinaryOperator* rightInit =(BinaryOperator*) decl->getInit();
	// 			if(rightInit->getOpcode()){
	// 				string texts("");
	// 				texts.append(type)
	// 				.append(" ")
	// 				.append(name)
	// 				.append(";\n")
	// 				.append(name)
	// 				.append(" = ")
	// 				.append(rewriter.getRewrittenText(getSourceRange(decl->getInit())))
	// 				.append("");
	// 				// rewriter.InsertTextAfter(getPureLocFirst(decl->getLocStart()), texts);
	// 				rewriter.ReplaceText(getSourceRange(decl), texts);
	// 				isChanged = true;
	// 			}
	// 		}
	// 		catch(double)
	// 		{
	// 			std::cerr<<"non Binops\n";
	// 		}
	// 	}
	// 	return true;
	// }
};

class ExtractStmtASTConsumer : public ASTConsumer
{
private:
	ExtractStmtVisitor *visitor; // doesn't have to be private

public:
	// override the constructor in order to pass CI
	explicit ExtractStmtASTConsumer(CompilerInstance *CI)
		: visitor(new ExtractStmtVisitor(CI)) // initialize the visitor
	{
	}

	// override this to call our ExtractStmtVisitor on the entire source file
	virtual void HandleTranslationUnit(ASTContext &Context)
	{
		/* we can use ASTContext to get the TranslationUnitDecl, which is
		a single Decl that collectively represents the entire source file */
		visitor->TraverseDecl(Context.getTranslationUnitDecl());
	}
};

class ExtractStmtFrontendAction : public ASTFrontendAction
{
public:
	virtual std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef file)
	{
		rewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
		return std::unique_ptr<ASTConsumer>(new ExtractStmtASTConsumer(&CI)); // pass CI pointer to ASTConsumer
	}
};

#endif
