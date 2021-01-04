#ifndef TACTRANSOPFORM_H
#define TACTRANSOPFORM_H

#include "../Headers.h"
#include "../Constants.h"
#include "../Utils.h"

bool FIRSTFLAG = true;
static bool binOpFunction = false;

class TACTransformVisitor : public RecursiveASTVisitor<TACTransformVisitor>
{
private:
	ASTContext *astContext; // used for getting additional AST info

	SourceLocation beginLoc;

	Stmt *containerStmt;

	bool ignore;

	bool hasCondOp;

	int varCount;

	int biExprCount;

	int callExprCount;

	VarInfo lastNewVar;

	vector<string> stats;

	vector<string> operatorStream;

	vector<string> operatorTmpArgStream;

	vector<string> operatorTmpArg;

	vector<string> operatorCallExpStream;

	map<string, string> operatorCallExp;
	string expstr = "";

	bool flagEqual;

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
			return VarInfo("(" + op + "(" + ret.var + "))", ret.fpVar, ret.vars);
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
			return VarInfo("((" + type + ")" + ret.var + ")", ret.fpVar, ret.vars);
		}
		// else if (ArraySubscriptExpr *specExpr = dyn_cast<ArraySubscriptExpr>(expr))
		// {
		// 	VarInfo ret = getVar(specExpr->getBase());
		// 	VarInfo retInd = getVar(specExpr->getIdx());
		// 	string ind = retInd.var;
		// 	string arr = ret.var + "[" + ind + "]";
		// 	retInd.vars.push_back(arr);
		// 	return VarInfo(arr, ret.fpVar, retInd.vars);
		// }
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
				return ret;
			}
		}
		else if (CallExpr *specExpr = dyn_cast<CallExpr>(expr))
		{
			string func = specExpr->getCalleeDecl()->getAsFunction()->getNameAsString();
			func.append("(");
			int numArgs = specExpr->getNumArgs();
			string callexptmp;
			VarInfo varInfo;
			for (int i = 0; i < numArgs; i++)
			{
				VarInfo arg = getVar(specExpr->getArg(i));
				func.append(arg.var);
				for (int i = 0; i < arg.vars.size(); i++)
				{

					varInfo.vars.push_back(arg.vars[i]);
				}
				if (operatorCallExp.find(arg.var) != operatorCallExp.end())
				{
					//			operatorCall	ExpStream.insert(operatorCallExpStream.begin(),operatorCallExp[leftVar.var]);
					operatorCallExpStream.push_back(operatorCallExp[arg.var]);
					callexptmp.append("/*embed fun has found*/\n");
				}
				if (i != numArgs - 1)
				{
					func.append(", ");
				}
			}
			func.append(")");

			string type = specExpr->getType().getAsString().data();
			if (type != "float" && type != "double" && type != "long double")
				varInfo.var = func;
			else
			{

				callexptmp.append(type);
				string callexptmpVar = callexpVar + toString(callExprtagCount++);
				callexptmp.append(" ").append(callexptmpVar).append(";\n");
				string callStatement = "";
				string callSC = "";
				callSC.append(callexptmpVar).append(" = ").append(func).append(";");
				stats.push_back(callSC);
				operatorStream.push_back(callStatement);
				callStatement = "";
				callSC = "";
				// operatorCallExpStream.push_back(callexptmp);
				operatorCallExp.insert(map<string, string>::value_type(callexptmpVar, callexptmp));
				varInfo.var = callexptmpVar;
				varInfo.isCallExp = true;
			}
			callExprCount++;
			return varInfo;
		}
		else if (DeclRefExpr *specExpr = dyn_cast<DeclRefExpr>(expr))
		{
			string ret = specExpr->getNameInfo().getAsString().data();
			cout << "------declref begin-------------" << endl;
			cout << "declref:" << ret << endl;
			string varType = "";
			if (VarDecl *VD = dyn_cast<VarDecl>(specExpr->getDecl()))
			{
				// It's a reference to a variable (a local, function parameter, global, or static data member).
				varType = VD->getType().getAsString(); // var type
			}

			cout << "------declref end-------------" << endl;
			VarInfo varInfo(ret);
			if (varType == "double" || varType == "float" || varType == "long double")
			{
				varInfo.isFloat = true;
				varInfo.floatType = varType;
			}
			varInfo.isTmpVar = false;
			varInfo.vars.push_back(ret);
			return varInfo;
		}
		else
		{
			/*if (isa<UnaryExprOrTypeTraitExpr>(*expr)) {
			cout << rewriter.getRewrittenText(expr->getSourceRange()) << endl;
			}*/
			if (isa<ConditionalOperator>(*expr))
			{
				biExprCount += countBinaryOp(expr);
			}
			string ret = rewriter.getRewrittenText(getSourceRange(expr));
			cout << "------ret end-------------" << endl;
			cout << "ret:" << ret << endl;
			cout << "------ret end-------------" << endl;
			//if (ret.empty()) {

			/*} else if (IntegerLiteral * specExpr = dyn_cast<IntegerLiteral>(expr)) {
				return VarInfo(specExpr->getValue().toString(10,true));
				} else if (FloatingLiteral * specExpr = dyn_cast<FloatingLiteral>(expr)) {
				hasDefine = true; // do not find a better way to deal with this case
				}*/
			//}
			//cout << "re" << rewriter.getRewrittenText(expr->getSourceRange()) << endl;
			VarInfo varInfo(ret);
			varInfo.isConst = true; // record the const
			if (!isLiteral && !isMacro(expr))
				varInfo.vars.push_back(ret);
			return varInfo;
		}
	}

	VarInfo threeAddrCode(BinaryOperator *biExpr)
	{
		Expr *lhs, *rhs;
		biExprCount++;

		VarInfo leftVar = getVar(biExpr->getLHS());
		VarInfo rightVar = getVar(biExpr->getRHS());

		if (leftVar.isCallExp)
		{
			//			operatorCall	ExpStream.insert(operatorCallExpStream.begin(),operatorCallExp[leftVar.var]);
			operatorCallExpStream.push_back(operatorCallExp[leftVar.var]);
		}
		if (rightVar.isCallExp)
		{
			//			operatorCallExpStream.insert(operatorCallExpStream.begin(),operatorCallExp[rightVar.var]);
			operatorCallExpStream.push_back(operatorCallExp[rightVar.var]);
		}
		string op = biExpr->getOpcodeStr().data();
		string type = biExpr->getType().getAsString();
		string statment("");
		string varName("");
		bool reduceLeftVar = true;
		vector<string> BinArgs(2);
		if (op == "=")
		{
			// string ltype = biExpr->getLHS()->getType().getAsString() ;
			// if(ltype != "float" && ltype != "double" && ltype != "long double")
			if (type == "double" || type == "float" || type == "long double")
			{
				statment += leftVar.var + " = " + rightVar.var + ";\n";
				vector<string>::iterator it;
				// for (it = operatorTmpArgStream.begin(); it != operatorTmpArgStream.end(); it++)
				// {
				// 	if (it == operatorTmpArgStream.begin())
				// 	{
				// 		if (FIRSTFLAG)
				// 		{
				// 			statment.append("RContext::setDefaultContext(new RContext(128, MPFR_RNDD));\n");
				// 			FIRSTFLAG = false;
				// 		}
				// 		statment.append("Real ");
				// 		statment.append((*it));
				// 	}
				// 	else
				// 		statment.append(" , ").append((*it));
				// }
				// if (operatorTmpArgStream.size() > 0)
				// 	statment.append(";\n");
				// operatorTmpArgStream.clear();
				for (it = operatorStream.begin(); it != operatorStream.end(); it++)
				{
					statment.append((*it)) + "ok";
				}
				operatorStream.clear();
				// if (flagEqual)
				// 	if (rightVar.isFloat)
				// 		statment = statment + "mapShadow((Addr)&" + leftVar.var + " , \"" + type + "\" , (Addr)&" + rightVar.var + " , \"" + toString(biExpr->getExprLoc()) + "\");"; //check c`s shadow and map rightVar.var to leftVar.var";
				// 	else
				// 		statment = statment + "mapShadow((Addr)&" + leftVar.var + " , \"" + type + "\" , " + rightVar.var + " , \"" + toString(biExpr->getExprLoc()) + "\");"; //check c`s shadow and map rightVar.var to leftVar.var";
				// else
				// 	statment = statment + "mapShadow((Addr)&" + leftVar.var + " , \"" + type + "\" , " + rightVar.var + "_real , \"" + toString(biExpr->getExprLoc()) + "\");"; //check c`s shadow and map rightVar.var to leftVar.var";
				statment.append(getAssignmentStr(biExpr->getType().getAsString())).append("({").append(LRVar(leftVar)).append("},").append(LRVar(rightVar)).append(",\"").append(toString(biExpr->getExprLoc())).append("\");");
				flagEqual = true;
				reduceLeftVar = false;
				binOpFunction = false;
			}
			else
			{
				statment = leftVar.var + " " + op + " " + rightVar.var + ";\n";
			}
			if (statment.size() > 0)
				stats.push_back(statment);
			VarInfo varInfo;
			varInfo.var = leftVar.var;
			varInfo.isFloat = true;
			return varInfo;
		}
		else if (isa<CompoundAssignOperator>(*biExpr))
		{
			statment = leftVar.var + op + rightVar.var + ";";
			flagEqual = false;
		}
		else
		{
			// if (type == "double" || type == "float" || type == "long double")
			// {
			varName = tempVar + toString(varCount);
			statment = varName + " = " + leftVar.var + " " + op + " " + rightVar.var + ";\n";
			string tmpStmt("");
			string tmpVarReal("");
			operatorTmpArg.push_back(varName);
			tmpVarReal = varName + "_real";
			// tmpStmt = tmpStmt + "compute(" + varName + "_real, (Addr)&" + varName + " , \"" + type + "\" , " + LRVarStr(leftVar) + " , " + LRVarStr(rightVar) + " , \"" + op + "\");// a op b\n"; // a op b
			string computeStr = getComputeStr(op, type);

			tmpStmt = tmpStmt + computeStr.append("((Addr)&").append(varName).append(",{").append(LRVar(leftVar) + " , " + LRVar(rightVar)).append("},\"").append(toString(biExpr->getExprLoc())).append("\");");

			operatorStream.push_back(tmpStmt);
			// operatorTmpArgStream.push_back(tmpVarReal);
			flagEqual = false;
			binOpFunction = true;
		}
		cout << "----------binop begin-----------" << endl;
		cout << "binop type " << type << "op " << op << endl;
		cout << "right " << biExpr->getRHS()->getType().getAsString() << " " << rightVar.var << endl;
		cout << "left " << biExpr->getLHS()->getType().getAsString() << " " << leftVar.var << endl;
		cout << "----------binop processing -----------" << endl;

		VarInfo varInfo;
		cout << "----------binop end -----------" << endl;
		stats.push_back(statment);
		varName = tempVar + toString(varCount);
		varInfo.var = varName;
		varCount++;
		varInfo.isTmpVar = true;
		varInfo.isFloat = true;
		return varInfo;
	}

	string process(BinaryOperator *biExpr)
	{

		string texts("");
		cout << "iii " << biExprCount << endl;
		if (biExprCount == 0)
		{
			lastNewVar = threeAddrCode(biExpr);
			biExpr->dump();
			vector<string>::iterator it;

			for (it = operatorTmpArg.begin(); it != operatorTmpArg.end(); it++)
			{
				if (it == operatorTmpArg.begin())
				{
					texts.append(biExpr->getType().getAsString()).append(" ");
					texts.append((*it));
				}
				else
				{
					texts.append(",").append((*it));
				}
			}
			if (operatorTmpArg.size())
			{
				texts.append(";");
			}
			for (it = operatorConstStream.begin(); it != operatorConstStream.end(); it++)
			{
				if (it == operatorConstStream.begin())
				{
					texts.append(biExpr->getType().getAsString()).append(" ");
					texts.append((*it));
				}
				else
				{
					texts.append(",").append((*it));
				}
			}
			if (operatorConstStream.size())
			{
				texts.append(";");
			}
			for (it = operatorCallExpStream.begin(); it != operatorCallExpStream.end(); it++)
			{
				texts.append((*it)).append("\n");
			}

			for (it = stats.begin(); it != stats.end(); it++)
			{
				texts.append((*it)).append("\n");
			}
			rewriter.ReplaceText(getSourceRange(biExpr), texts);
			//rewriter.InsertTextAfter(biExpr->getExprLoc () , texts);
			isChanged = true;
			operatorCallExpStream.clear();
			operatorTmpArg.clear();
			stats.clear();
			operatorConstStream.clear();
		}
		cout << "----------binop end -----------" << endl;
		biExprCount--;
		return texts;
	}

public:
	explicit TACTransformVisitor(CompilerInstance *CI)
		: astContext(&(CI->getASTContext())) // initialize private members
	{
		ignore = false;
		varCount = 0;
		biExprCount = 0;
		callExprCount = 0;
		flagEqual = true;
		binOpFunction = false;
		ifstream in(logfile);
		in >> tagCount;
		in.close();
	}

	virtual bool VisitBinaryOperator(BinaryOperator *biOp)
	{
		if (ignore)
			return true;
		string type = biOp->getLHS()->getType().getAsString();
		if (type == "double" || type == "float" || type == "long double")
			process(biOp);
		return true;
	}

	virtual bool TraverseForStmt(ForStmt *stmt)
	{
		if (!WalkUpFromForStmt(stmt))
			return false;
		bool preIgnore = ignore;
		for (Stmt::child_range range = stmt->children(); range; ++range)
		{
			if (*range == stmt->getBody())
			{
				if (!preIgnore)
				{
					ignore = false;
				}
			}
			else
				ignore = true;
			TraverseStmt(*range);
		}
		ignore = preIgnore;
		return true;
	}

	virtual bool TraverseCaseStmt(CaseStmt *stmt)
	{
		if (!WalkUpFromCaseStmt(stmt))
			return false;
		bool preIgnore = ignore;
		for (Stmt::child_range range = stmt->children(); range; ++range)
		{
			if (*range == stmt->getLHS())
				ignore = true;
			else if (!preIgnore)
			{
				ignore = false;
			}
			TraverseStmt(*range);
		}
		ignore = preIgnore;
		return true;
	}

	virtual bool TraverseCallExpr(CallExpr *stmt)
	{
		if (!WalkUpFromCallExpr(stmt))
			return false;
		bool preIgnore = ignore;
		for (Stmt::child_range range = stmt->children(); range; ++range)
		{
			if (*range == stmt->getCallee())
			{

				if (!preIgnore)
				{
					ignore = true;
				}
			}
			else
			{
				ignore = true;
			}

			if (callExprCount != 0) // visit before
			{
				ignore = false;
			}

			TraverseStmt(*range);
		}
		ignore = preIgnore;
		if (callExprCount > 0)
		{
			callExprCount--;
		}
		return true;
	}
	// 	virtual bool VisitCallExpr(CallExpr *callexp){
	// 	if(binOpFunction )//
	// 	{
	// 		string type = callexp->getType().getAsString().data();

	// 		string varName = extractVar + toString(varCount);

	// 		string texts("\n/*func*/\n");
	// 		texts.append(type).append(" ")
	// 		.append(varName).append(" = ")
	// 		.append(rewriter.getRewrittenText(getSourceRange(callexp)))
	// 		.append(";\n");
	// 		rewriter.InsertTextBefore(getPureLocFirst(callexp->getLocStart()),texts);
	// 		return true;
	// 	}
	// }
};

class TACTransformASTConsumer : public ASTConsumer
{
private:
	TACTransformVisitor *visitor; // doesn't have to be private

public:
	// override the constructor in order to pass CI
	explicit TACTransformASTConsumer(CompilerInstance *CI)
		: visitor(new TACTransformVisitor(CI)) // initialize the visitor
	{
	}

	// override this to call our TACTransformVisitor on the entire source file
	virtual void HandleTranslationUnit(ASTContext &Context)
	{
		/* we can use ASTContext to get the TranslationUnitDecl, which is
		a single Decl that collectively represents the entire source file */
		visitor->TraverseDecl(Context.getTranslationUnitDecl());
	}
};

class TACTransformFrontendAction : public ASTFrontendAction
{
public:
	virtual std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef file)
	{
		rewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
		return std::unique_ptr<ASTConsumer>(new TACTransformASTConsumer(&CI)); // pass CI pointer to ASTConsumer
	}
};

#endif
