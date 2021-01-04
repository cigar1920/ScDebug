#ifndef TACTRANSFORM_H
#define TACTRANSFORM_H

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

	vector<string> operatorCallexpStream;

	map<string, string> operatorCallExp;

	queue<string> operatorTmpArg;

	string expStr = "";

	string lastOp = "";

	string lastType = "";

	string lastPro = "";

	string lastTem = "";

	string constTem = "double ";

	string paraStr = "";

	string equalityOp = "";

	string typeStr = "";

	vector<string> equalityStr;

	string computeStr = "";

	bool flagEqual;

	CompilerInstance *TCI = (CompilerInstance *)malloc(sizeof(CompilerInstance));

	string getLeast()
	{

		string statment;
		string varName = tempVar + toString(varCount);
		statment = lastType + " " + varName + " ;";
		lastTem = varName;
		varCount++;
		computeStr = getComputeStr(lastOp, lastType); // type is the lastop type
		statment.append(computeStr).append("((Addr)&").append(varName).append(",{").append(paraStr).append("},\"").append("toString(biExpr->getExprLoc())").append("\");");
		lastPro = "";
		lastOp = "";
		lastType = "";
		return statment;
	}
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
		bool isFPNum = false;
		string type = expr->getType().getAsString().data();
		if (type == "float" || type == "double" || type == "long double")
			isFPNum = true;
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
			if (specExpr->getOpcode() >= 2)
				return VarInfo("(" + op + "(" + ret.var + "))", ret.fpVar, ret.vars, true, isFPNum);
			return VarInfo("((" + ret.var + ")" + op + ")", ret.fpVar, ret.vars, true, isFPNum);
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

			VarInfo ret = getVar(specExpr->getSubExpr());

			ret.isFloat = true;
			return VarInfo("((" + type + ")" + ret.var + ")", ret.fpVar, ret.vars, false, isFPNum);
		}
		// else if (ArraySubscriptExpr *specExpr = dyn_cast<ArraySubscriptExpr>(expr))
		// {
		// 	VarInfo ret = getVar(specExpr->getBase());
		// 	VarInfo retInd = getVar(specExpr->getIdx());
		// 	string ind = retInd.var;
		// 	string arr = ret.var + "[" + ind + "]";
		// 	retInd.vars.push_back(arr);
		// 	ret.isFloat = true;
		// 	return VarInfo(arr, ret.fpVar, retInd.vars, false, isFPNum);
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
				ret.isCallExp = true;
				ret.isConst = false;
				ret.isFloat = true;
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
				ret.isCallExp = true;
				ret.isConst = false;
				ret.isFloat = true;
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
			string type = specExpr->getType().getAsString().data();
			for (int i = 0; i < numArgs; i++)
			{

				VarInfo arg = getVar(specExpr->getArg(i));
				if (i == numArgs - 1)
				{
					if (lastPro.size() > 0)
					{
						// process();
						string statment;
						string varName = tempVar + toString(varCount);
						operatorTmpArgStream.push_back(varName);
						lastTem = varName;
						varCount++;
						string binExperStr = lastType + " " + varName + " = " + lastPro + ";";
						computeStr = getComputeStr(lastOp, lastType); // type is the lastop type
						statment.append(computeStr).append("((Addr)&").append(varName).append(",{").append(paraStr).append("},\"").append("toString(biExpr->getExprLoc())").append("\");");
						operatorStream.push_back(statment);
						stats.push_back(binExperStr);
						statment = "";
						lastPro = "";
						lastOp = "";
						lastType = "";
					}
				}
				func.append(arg.var);
				for (int i = 0; i < arg.vars.size(); i++)
				{

					varInfo.vars.push_back(arg.vars[i]);
				}
				if (operatorCallExp.find(arg.var) != operatorCallExp.end())
				{
					//			operatorCall	expStream.insert(operatorCallexpStream.begin(),operatorCallExp[leftVar.var]);
					operatorCallexpStream.push_back(operatorCallExp[arg.var]);
					callexptmp.append("/*embed fun has found*/\n");
				}
				if (i != numArgs - 1)
				{
					func.append(", ");
				}
			}
			func.append(")");

			if (type != "float" && type != "double" && type != "long double")
				varInfo.var = func;
			else
			{
				callexptmp.append(type);
				string callexptmpVar = callexpVar + toString(callExprtagCount++);
				callexptmp.append(" ").append(callexptmpVar).append(";\n").append(callexptmpVar).append(" = ").append(func).append(";\n");
				callexptmp.append("CallStackPop((Addr)&").append(callexptmpVar).append(",getTop(").append(callexptmpVar).append("));\n");

				// callexptmp.append("\n//insert the stack option to get the real num (it need to free the top and its shadow)\n");
				// operatorCallexpStream.push_back(callexptmp);
				operatorCallExp.insert(map<string, string>::value_type(callexptmpVar, callexptmp));
				varInfo.var = callexptmpVar;
				varInfo.isCallExp = true;
				varInfo.isFloat = true;
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
			if (isa<ConditionalOperator>(*expr))
			{
				biExprCount += countBinaryOp(expr);
			}
			string ret = rewriter.getRewrittenText(getSourceRange(expr));
			cout << "------ret end-------------" << endl;
			cout << "ret:" << ret << endl;
			cout << "------ret end-------------" << endl;
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
		VarInfo varInfo;
		VarInfo leftVar = getVar(biExpr->getLHS());
		VarInfo rightVar = getVar(biExpr->getRHS());
		string leftVarStr, rightVarStr;
		leftVarStr = leftVar.var;
		rightVarStr = rightVar.var;
		cout << "&&&&&&&&&&&&&&&&" << endl;
		cout << "op" << leftVarStr << biExpr->getOpcodeStr().data() << rightVarStr << " " << lastPro << endl;
		cout << "&&&&&&&&&&&&&&&&" << endl;

		if (leftVar.isCallExp)
		{
			//			operatorCall	expStream.insert(operatorCallexpStream.begin(),operatorCallExp[leftVar.var]);
			//operatorCallexpStream.push_back(operatorCallExp[leftVar.var]);
			stats.push_back(operatorCallExp[leftVar.var]);
		}
		if (rightVar.isCallExp)
		{
			//			operatorCallexpStream.insert(operatorCallexpStream.begin(),operatorCallExp[rightVar.var]);
			//operatorCallexpStream.push_back(operatorCallExp[rightVar.var]);
			stats.push_back(operatorCallExp[rightVar.var]);
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
				if (equalityOp == "")
					equalityOp = lastOp;
				if (lastPro == "") // equlity
				{
					if (lastOp == "=" && stats.size() > 0)
						stats[stats.size() - 1] = leftVarStr + " = " + stats[stats.size() - 1];
					else
					{
						statment += leftVarStr + " = " + rightVarStr + ";";
						paraStr = LRVar(rightVar);
					}
					lastPro = "";
					lastOp = "=";
					lastType = type;
					equalityStr.push_back(leftVarStr);
				}
				else // copy value to a
				{
					statment += leftVarStr + " = " + lastPro + ";";
					computeStr = getComputeStr(lastOp, type);
					for (vector<string>::iterator it = operatorStream.begin(); it != operatorStream.end(); it++)
					{
						statment.append((*it));
					}
					operatorStream.clear();
					statment.append(computeStr).append("((Addr)&").append(leftVarStr).append(",{").append(paraStr).append("},\"").append(toString(biExpr->getExprLoc())).append("\");");

					lastPro = "";
					lastOp = "";
					lastType = type;
				}
				varInfo.var = leftVarStr;

				reduceLeftVar = false;
				binOpFunction = false;
			}
			else
			{
				statment = leftVar.var + " " + op + " " + rightVar.var + ";";
			}
			if (statment.size() > 0)
				stats.push_back(statment);
			varInfo.isFloat = true;
			varInfo.isTmpVar = false;
			lastType = type;
			return varInfo;
		}
		else if (isa<CompoundAssignOperator>(*biExpr))
		{
			statment = leftVar.var + op + rightVar.var + ";";
			flagEqual = false;
		}
		else
		{
			if (type == "double" || type == "float" || type == "long double")
			{

				cout << lastOp << op << endl;
				if ((op == "+"))
				{
					if ((lastOp == "+")) // Equal priority operation
					{
						if (lastPro.size() == 0)
						{
							lastPro.append(leftVarStr + " " + op + " " + rightVarStr);
							paraStr = LRVar(leftVar) + "," + LRVar(rightVar);
							lastOp = op;
						}
						else
						{
							if ((leftVarStr.substr(0, 17) != tempVar) && (rightVarStr.substr(0, 17) != tempVar)) //include the () to change the priority
							{
								varName = tempVar + toString(varCount);
								lastTem = varName;
								statment = type + " " + varName + " = " + lastPro + ";";

								computeStr = getComputeStr(lastOp, type);
								string cmpuStr = "";
								cmpuStr.append(computeStr).append("((Addr)&").append(varName).append(",{").append(paraStr).append("},\"").append(toString(biExpr->getExprLoc())).append("\");");
								operatorStream.push_back(cmpuStr);
								cmpuStr = "";

								paraStr = LRVar(leftVar) + "," + LRVar(rightVar);

								lastPro = leftVarStr + " " + op + " " + rightVarStr;

								lastOp = op;
								binOpFunction = true;
								varCount++;
								varInfo.vars.push_back(varName);
								varInfo.var = varName;
								varInfo.isTmpVar = false;
							}
							else if (leftVarStr.substr(0, 17) != tempVar)
							{
								paraStr += "," + LRVar(leftVar);
								lastPro.append(op + " " + leftVarStr);
							}
							else if (rightVarStr.substr(0, 17) != tempVar)
							{
								paraStr += "," + LRVar(rightVar);
								lastPro.append(op + " " + rightVarStr);
							}
							else
							{
								paraStr = LRVar(leftVar) + "," + paraStr;
								lastPro = leftVarStr + " " + op + " " + lastPro;
							}
						}
					}
					else
					{

						if (lastPro == "")
						{
							lastPro = statment = leftVarStr + " " + op + " " + rightVarStr;
							//statment.append("compute( \'"+ lastOp +"\',(Addr)&").append(varName).append(",{").append(paraStr).append("},\"").append(toString(biExpr->getExprLoc())).append("\");");

							paraStr = LRVar(leftVar) + "," + LRVar(rightVar);

							lastOp = op;
							varInfo.isFloat = true;
							varInfo.isTmpVar = false;
							binOpFunction = true;
							varInfo.var = tempVar + toString(varCount);
							lastType = type;
							return varInfo;
						}
						else
						{
							varName = tempVar + toString(varCount);
							lastTem = varName;
							statment = type + " " + varName + " = " + lastPro + ";";
							computeStr = getComputeStr(lastOp, type);
							string cmpuStr = "";
							cmpuStr.append(computeStr).append("((Addr)&").append(varName).append(",{").append(paraStr).append("},\"").append(toString(biExpr->getExprLoc())).append("\");");
							operatorStream.push_back(cmpuStr);
							cmpuStr = "";
							lastPro = leftVarStr + " " + op + " " + rightVarStr;
							paraStr = LRVar(leftVar) + "," + LRVar(rightVar);
							lastOp = op;
							binOpFunction = true;
							varCount++;
							varInfo.vars.push_back(varName);
							varInfo.var = varName;
							varInfo.isTmpVar = false;
						}
					}
				}
				else if (op == "-")
				{
					if (lastPro.size() == 0)
					{
						lastPro.append(leftVarStr + " " + op + " " + rightVarStr);
						paraStr = LRVar(leftVar) + "," + LRVar(rightVar);
						lastOp = op;
					}
					else
					{
						varName = tempVar + toString(varCount);
						lastTem = varName;

						statment = type + " " + varName + " = " + lastPro + ";";
						computeStr = getComputeStr(lastOp, type);
						string cmpuStr = "";
						cmpuStr.append(computeStr).append("((Addr)&").append(varName).append(",{").append(paraStr).append("},\"").append(toString(biExpr->getExprLoc())).append("\");");
						operatorStream.push_back(cmpuStr);
						cmpuStr = "";
						lastPro = leftVarStr + " " + op + " " + rightVarStr;
						paraStr = LRVar(leftVar) + "," + LRVar(rightVar);
						lastOp = op;
						binOpFunction = true;
						varCount++;
						varInfo.vars.push_back(varName);
						varInfo.var = varName;
						varInfo.isTmpVar = false;
					}
				}
				else if (op == "*")
				{
					if ((lastOp == "*")) // Equal priority operation
					{
						if (lastPro.size() == 0)
						{
							lastPro.append(leftVarStr + " " + op + " " + rightVarStr);
							paraStr = LRVar(leftVar) + "," + LRVar(rightVar);
							lastOp = op;
						}
						else
						{
							if ((leftVarStr.substr(0, 17) != tempVar) && (rightVarStr.substr(0, 17) != tempVar)) //include the () to change the priority
							{
								varName = tempVar + toString(varCount);
								lastTem = varName;
								statment = type + " " + varName + " = " + lastPro + ";";
								computeStr = getComputeStr(lastOp, type);
								string cmpuStr = "";
								cmpuStr.append(computeStr).append("((Addr)&").append(varName).append(",{").append(paraStr).append("},\"").append(toString(biExpr->getExprLoc())).append("\");");
								operatorStream.push_back(cmpuStr);
								cmpuStr = "";
								lastPro = leftVarStr + " " + op + " " + rightVarStr;
								paraStr = LRVar(leftVar) + "," + LRVar(rightVar);
								lastOp = op;
								binOpFunction = true;
								varCount++;
								varInfo.vars.push_back(varName);
								varInfo.var = varName;
								varInfo.isTmpVar = false;
							}
							else if (leftVarStr.substr(0, 17) != tempVar)
							{
								lastPro.append(op + " " + leftVarStr);
								paraStr += "," + LRVar(leftVar);
							}
							else if (rightVarStr.substr(0, 17) != tempVar)
							{
								lastPro.append(op + " " + rightVarStr);
								paraStr += "," + LRVar(rightVar);
							}
							else
							{
								lastPro = leftVarStr + " " + op + " " + lastPro;
								paraStr = LRVar(leftVar) + "," + paraStr;
							}
						}
					}
					else
					{
						if (lastPro == "")
						{
							lastPro = statment = leftVarStr + " " + op + " " + rightVarStr;
							//statment.append("compute( \'"+ lastOp +"\',(Addr)&").append(varName).append(",{").append(paraStr).append("},\"").append(toString(biExpr->getExprLoc())).append("\");");

							paraStr = LRVar(leftVar) + "," + LRVar(rightVar);
							lastOp = op;
							varInfo.isFloat = true;
							varInfo.isTmpVar = false;
							binOpFunction = true;
							varInfo.var = tempVar + toString(varCount);
							lastType = type;
							return varInfo;
						}
						else
						{
							varName = tempVar + toString(varCount);
							lastTem = varName;
							statment = type + " " + varName + " = " + lastPro + ";";
							computeStr = getComputeStr(lastOp, type);
							string cmpuStr = "";
							cmpuStr.append(computeStr).append("((Addr)&").append(varName).append(",{").append(paraStr).append("},\"").append(toString(biExpr->getExprLoc())).append("\");");
							operatorStream.push_back(cmpuStr);
							cmpuStr = "";
							lastPro = leftVarStr + " " + op + " " + rightVarStr;
							paraStr = LRVar(leftVar) + "," + LRVar(rightVar);
							lastOp = op;
							binOpFunction = true;
							varCount++;
							varInfo.vars.push_back(varName);
							varInfo.var = varName;
							varInfo.isTmpVar = false;
						}
					}
				}
				else
				{
					if (lastPro.size() == 0)
					{
						lastPro.append(leftVarStr + " " + op + " " + rightVarStr);
						paraStr = LRVar(leftVar) + "," + LRVar(rightVar);
						lastOp = op;
					}
					else
					{
						varName = tempVar + toString(varCount);
						lastTem = varName;

						statment = type + " " + varName + " = " + lastPro + ";";
						computeStr = getComputeStr(lastOp, type);
						string cmpuStr = "";
						cmpuStr.append(computeStr).append("((Addr)&").append(varName).append(",{").append(paraStr).append("},\"").append(toString(biExpr->getExprLoc())).append("\");");
						operatorStream.push_back(cmpuStr);
						cmpuStr = "";
						lastPro = leftVarStr + " " + op + " " + rightVarStr;
						paraStr = LRVar(leftVar) + "," + LRVar(rightVar);
						lastOp = op;
						binOpFunction = true;
						varCount++;
						varInfo.vars.push_back(varName);
						varInfo.var = varName;
						varInfo.isTmpVar = false;
					}
				}
				flagEqual = false;
				lastOp = op;
			}
		}
		cout << "----------binop begin-----------" << endl;
		cout << "binop type " << type << " op " << op << endl;
		cout << "right " << biExpr->getRHS()->getType().getAsString() << " " << rightVar.var << endl;
		cout << "left " << biExpr->getLHS()->getType().getAsString() << " " << leftVar.var << endl;
		cout << "----------binop processing -----------" << endl;
		cout << "----------binop end -----------" << endl;
		if (statment.size() > 0)
			stats.push_back(statment);
		varInfo.isFloat = true;
		varName = tempVar + toString(varCount);
		varInfo.var = varName;
		lastType = type;
		varInfo.isTmpVar = false;
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
			for (it = operatorCallexpStream.begin(); it != operatorCallexpStream.end(); it++)
			{
				texts.append((*it)).append("\n");
			}
			for (it = stats.begin(); it != stats.end(); it++)
			{
				texts.append((*it)).append("\n");
			}

			if (equalityStr.size() > 0)
			{
				string s("");
				string equalityVar = ("");
				for (int i = 0; i < equalityStr.size(); i++)
				{
					if (i == 0)
						equalityVar.append("(Addr)&").append(equalityStr[i]);
					else
						equalityVar.append(",(Addr)&").append(equalityStr[i]);
				}
				texts.append(getAssignmentStr(biExpr->getType().getAsString())).append("({").append(equalityVar).append("},").append(paraStr).append(",\"").append(toString(biExpr->getExprLoc())).append("\");");
			}
			SourceLocation endOfStmt = clang::Lexer::findLocationAfterToken(biExpr->getLocEnd(),
																			tok::r_brace, TCI->getSourceManager(), TCI->getLangOpts(), false);
			if (endOfStmt.isValid())
			{
				if (lastPro.size() > 0)
					texts.append(getLeast());
				// rewriter.InsertTextAfter(endOfStmt, ";" + texts); //biExpr->getLocEnd() , texts);
				// cout << "- end -" << endOfStmt.printToString(TCI->getSourceManager()) << endl;
			}
			equalityStr.clear();
			operatorTmpArgStream.clear();
			rewriter.ReplaceText(getSourceRange(biExpr), texts);
			// rewriter.InsertTextAfter(biExpr->getExprLoc () , texts);
			// SourceLocation endOfStmt = clang::Lexer::findLocationAfterToken(biExpr->getLocEnd(),
			// 		tok::semi, TCI->getSourceManager(), TCI->getLangOpts(), false);
			// if (endOfStmt.isValid())
			// {
			// 	rewriter.InsertTextAfter(endOfStmt, texts); //biExpr->getLocEnd() , texts);
			// 	cout<<"- end -"<<endOfStmt.printToString(TCI->getSourceManager())<<endl;
			// }
			isChanged = true;
			operatorConstStream.clear();
			operatorCallexpStream.clear();
			stats.clear();
		}
		cout << "----------binop end -----------" << endl;
		biExprCount--;
		return texts;
	}

public:
	explicit TACTransformVisitor(CompilerInstance *CI)
		: astContext(&(CI->getASTContext())) // initialize private members
	{
		TCI = CI;
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
		// else
		// {
		// 	lastPro = "";
		// 	lastOp = "";
		// 	operatorConstStream.clear();
		// 	operatorCallexpStream.clear();
		// 	stats.clear();
		// }
		return true;
	}

	virtual bool TraverseCompoundStmt(CompoundStmt *stmt)
	{
		if (!WalkUpFromCompoundStmt(stmt))
			return false;
		bool preIgnore = ignore;
		for (Stmt::child_range range = stmt->children(); range; ++range)
		{
			TraverseStmt(*range);
		}
		if (lastPro.size() > 0)
		{
			string texts = getLeast();
			rewriter.InsertTextBefore(stmt->getLocEnd(), texts);
		}
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
