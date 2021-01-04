
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "Headers.h"

Rewriter rewriter;
static cl::OptionCategory MyToolCategory("");

 
//log
const string logfile("tac.log");
int tagCount;
int callExprtagCount = 0;
static int constCount = 0;
static int postInc = 0;
bool isChanged = false;
bool isForStmtComma = false;
const string errorLog("err.log");
static vector<string> operatorConstStream;


//Real
const string realTemp = "_real";
const string constVar = "temp_var_for_const_";
const string printError = "computeErr";
const string printReduce = "reducePrec";
const string printResume = "resumePrec";



//variable name
const string tempVar = "temp_var_for_tac_";
const string extractVar = "temp_var_for_ext_";
const string coopVar = "temp_var_for_coop_";
const string callexpVar = "temp_var_for_callexp_";
const string postIncVar = "temp_var_for_postinc_";
static string equalLeft("");
//file suffix
const string ip = "_p.c";
const string uco = "_u.c";
const string es = "_e.c";
const string tac = "_tac.cpp";

#endif