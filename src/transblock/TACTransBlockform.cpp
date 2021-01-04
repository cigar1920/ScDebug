/*
 * @Author: your name
 * @Date: 2020-10-14 16:29:02
 * @LastEditTime: 2020-10-14 16:29:29
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \cppc:\Users\Cigar\Documents\NetSarang Computer\6\Xftp\Temporary\TACTransBlockform.cpp
 */

#include "TACTransBlockform.h"

int main(int argc, const char **argv) {
	CommonOptionsParser op(argc, argv, MyToolCategory);        
	ClangTool Tool(op.getCompilations(), op.getSourcePathList());
	int result = Tool.run(newFrontendActionFactory<TACTransformFrontendAction>().get());
	saveTagCountToLog();
	if (isChanged) {
		freopen((op.getSourcePathList().front() + tac).c_str(), "w", stdout);
		cout<<"#include \"../ScDebug/scdebug.h\""<<endl;
		rewriter.getEditBuffer(rewriter.getSourceMgr().getMainFileID()).write(outs());
	}
	return 0;
}
