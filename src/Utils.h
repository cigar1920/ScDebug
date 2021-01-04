#ifndef UTILS_H
#define UTILS_H

#include "Headers.h"
#include "Constants.h"
class VarInfo
{
public:
	VarInfo()
	{
		this->var = "";
		this->fpVar = "";
		this->isConst = false;
		this->isFloat = false;
		this->isTmpVar = false;
		this->isCallExp = false;
		floatType = "";
	}
	VarInfo(string var)
	{
		this->var = var;
		fpVar = "";
		this->isConst = false;
		this->isFloat = false;
		this->isTmpVar = false;
		this->isCallExp = false;
		floatType = "";
	}
	VarInfo(string var, string fpVar)
	{
		this->var = var;
		this->fpVar = fpVar;
		this->isConst = false;
		this->isFloat = false;
		this->isTmpVar = false;
		this->isCallExp = false;
		floatType = "";
	}
	VarInfo(string var, string fpVar, vector<string> &vars)
	{
		this->var = var;
		this->fpVar = fpVar;
		this->vars = vars;
		this->isConst = false;
		this->isFloat = false;
		this->isTmpVar = false;
		this->isCallExp = false;
		floatType = "";
	}
	VarInfo(string var, string fpVar, vector<string> &vars, bool isConst)
	{
		this->var = var;
		this->fpVar = fpVar;
		this->vars = vars;
		this->isConst = true;
		this->isFloat = false;
		this->isTmpVar = false;
		this->isCallExp = false;
		floatType = "";
	}
	VarInfo(string var, string fpVar, vector<string> &vars, bool isConst,bool isFloat)
	{
		this->var = var;
		this->fpVar = fpVar;
		this->vars = vars;
		this->isConst = isConst;
		this->isFloat = isFloat;
		this->isTmpVar = false;
		this->isCallExp = false;
		floatType = "";
	}
	string var;
	bool isConst;
	bool isFloat;
	bool isTmpVar;
	bool isCallExp;
	string floatType;
	string fpVar; // for union (mynumber), when access mynumber.i[], compute error for mynumber.d
	vector<string> vars;
};
class CondInfo
{
public:
	CondInfo()
	{
		decl = "";
		stmt = "";
		varName = "";
	}
	string decl;
	string stmt;
	string varName;
};

string toString(int a)
{
	ostringstream ost;
	ost << a;
	string str(ost.str());
	return str;
}

string toString(double a)
{
	ostringstream ost;
	ost << a;
	string str(ost.str());
	return str;
}


inline string toString(SourceLocation loc)
{
	return loc.printToString(rewriter.getSourceMgr());
}

inline string getColumn(SourceLocation loc)
{
	string str = loc.printToString(rewriter.getSourceMgr());
	str = str.substr(0,str.find_last_of(":"));
	int lastIndex = str.find_last_of(":");
	return str.substr(lastIndex+1,str.length()-lastIndex-1);
}

string FDErrorStr(string var, string tag, string type)
{
	string ret("");
	string tagNum = tag;
	if (tag[tag.size() - 1] == '_')
		tagNum = tag.substr(0, tag.size() - 1);
	ret = "\n" + printError + type + "(\"" + var + "_tag" + tag + "\", (Addr)&" + var + ", " + tagNum + ");";
	return ret;
}

string FDReduceStr(string var, string tag, string type)
{
	string ret("");
	string tagNum = tag;
	if (tag[tag.size() - 1] == '_')
		tagNum = tag.substr(0, tag.size() - 1);
	ret = "\n" + printReduce + type + "((Addr)&" + var + ", " + tagNum + ");";
	return ret;
}

string FDResumeStr(string var, string tag, string type)
{
	string ret("");
	ret = "\n" + printResume + type + "((Addr)&" + var + ", " + tag + ");";
	return ret;
}

string LRVarStr(VarInfo varInfo)
{
	string ret("");
	if (varInfo.isTmpVar) //temp Var
	{
		ret = varInfo.var + "_real";
	}
	else if (varInfo.isFloat) //float point Var
	{
		ret = "(Addr)&" + varInfo.var + " , \"" + varInfo.floatType + "\"";
	}
	else if (varInfo.isConst) //const
	{
		ret = varInfo.var;
	}
	else if (varInfo.isCallExp)
	{
		ret = "getTop(" + varInfo.var + ")";
	}
	else //(other type)
	{
		ret = "(double)" + varInfo.var;
	}
	return ret;
}

string LRVar(VarInfo var)
{
	//(Addr)&c, (Addr)&d, (Addr)&e, (Addr)&f,(Addr)&(a),(Addr)&(cons_num_0=1.1)
	string ret("");
	if (!var.isConst && var.isFloat)
	{
		ret = "(Addr)&" + var.var;
	}
	else
	{
		ret = "(Addr)&(" + constVar + toString(constCount) + "=" + var.var + ")";
		operatorConstStream.push_back(constVar + toString(constCount));
		constCount++;
	}
	return ret;
}

string getOpStr(string op)
{
	string OpStr("");
	if (op == "+")
	{
		OpStr.append("Add");
	}
	else if (op == "-")
	{
		OpStr.append("Sub");
	}
	else if (op == "*")
	{
		OpStr.append("Mul");
	}
	else if (op == "/")
	{
		OpStr.append("Div");
	}
	else if (op == ",")
	{
		OpStr.append("Com");
	}

	return OpStr;
}
string getComputeStr(string op, string compType)
{
	string computeStr("compute");
	if (compType == "float")
	{
		computeStr.append("F");
	}
	else if (compType == "double")
	{
		computeStr.append("D");
	}
	else if (compType == "long double")
	{
		computeStr.append("Ld");
	}
	computeStr.append(getOpStr(op));
	return computeStr;
}
string getAssignmentStr(string compType)
{
	string assignmentStr("Assign");
	if (compType == "float")
	{
		assignmentStr.append("F");
	}
	else if (compType == "double")
	{
		assignmentStr.append("D");
	}
	else if (compType == "long double")
	{
		assignmentStr.append("Ld");
	}
	return assignmentStr;
}

string getReMap(string compType)
{
	string assignmentStr("ReMap");
	if (compType == "float")
	{
		assignmentStr.append("F");
	}
	else if (compType == "double")
	{
		assignmentStr.append("D");
	}
	else if (compType == "long double")
	{
		assignmentStr.append("Ld");
	}
	return assignmentStr;
}

void saveTagCountToLog()
{
	ofstream out(logfile);
	out << tagCount;
	out.close();
}

#define SL_INVALID 0
#define SL_GREAT 1
#define SL_LESS 2
#define SL_EQUAL 3

int SLcmp(SourceLocation a, SourceLocation b)
{
	PresumedLoc PLoca = rewriter.getSourceMgr().getPresumedLoc(a);
	PresumedLoc PLocb = rewriter.getSourceMgr().getPresumedLoc(b);
	if (PLoca.getFilename() != PLocb.getFilename())
		return SL_INVALID;
	if (PLoca.getLine() > PLocb.getLine())
		return SL_GREAT;
	else if (PLoca.getLine() < PLocb.getLine())
		return SL_LESS;
	else
	{
		if (PLoca.getColumn() > PLocb.getColumn())
			return SL_GREAT;
		else if (PLoca.getColumn() < PLocb.getColumn())
			return SL_LESS;
		else
			return SL_EQUAL;
	}
}

SourceLocation getPureLocFirst(SourceLocation loc)
{
	cout << "before" << endl;
	loc.dump(rewriter.getSourceMgr());
	cout << endl;
	if (loc.isMacroID())
	{
		pair<SourceLocation, SourceLocation> expansionRange =
			rewriter.getSourceMgr().getImmediateExpansionRange(loc);
		cout << "after" << endl;
		expansionRange.first.dump(rewriter.getSourceMgr());
		cout << endl;
		return expansionRange.first;
	}
	return loc;
}

SourceLocation getPureLocSecond(SourceLocation loc)
{
	if (loc.isMacroID())
	{
		pair<SourceLocation, SourceLocation> expansionRange =
			rewriter.getSourceMgr().getImmediateExpansionRange(loc);
		return expansionRange.second;
	}
	return loc;
}

// Deal with macro
SourceRange getSourceRange(SourceLocation startLoc, SourceLocation endLoc)
{
	startLoc = getPureLocFirst(startLoc);
	endLoc = getPureLocSecond(endLoc);
	SourceRange expandedLoc(startLoc, endLoc);
	return expandedLoc;
}

SourceRange getSourceRange(Stmt *stmt)
{
	assert(stmt != NULL);
	return getSourceRange(stmt->getLocStart(), stmt->getLocEnd());
}

SourceRange getSourceRange(VarDecl *decl)
{
	assert(decl != NULL);
	return getSourceRange(decl->getLocStart(), decl->getLocEnd());
}

bool isSameFile(SourceLocation srcLoc)
{
	if (rewriter.getSourceMgr().getFileID(getPureLocFirst(srcLoc)) == rewriter.getSourceMgr().getMainFileID())
		return true;
	else
		return false;
}

bool isMacro(Stmt *stmt)
{
	if (stmt->getLocStart().isMacroID() || stmt->getLocEnd().isMacroID())
	{
		return true;
	}
	return false;
}

bool isLiteral(Stmt *stmt)
{
	if (IntegerLiteral *specExpr = dyn_cast<IntegerLiteral>(stmt))
		return true;
	if (FloatingLiteral *specExpr = dyn_cast<FloatingLiteral>(stmt))
		return true;
	if (CharacterLiteral *specExpr = dyn_cast<CharacterLiteral>(stmt))
		return true;
	if (ImaginaryLiteral *specExpr = dyn_cast<ImaginaryLiteral>(stmt))
		return true;
	if (StringLiteral *specExpr = dyn_cast<StringLiteral>(stmt))
		return true;
	return false;
}

bool containPara(string des, string lastpro)
{
	string pattern = "(" + des + ")";
	std::size_t found = lastpro.find(pattern);
	if (found != std::string::npos)
	{
		// char jiaojian = lastpro[found + pattern.size()+1];
		// if( jiaojian == ','  || jiaojian == '}')
		return true;
	}
	return false;
}
#endif