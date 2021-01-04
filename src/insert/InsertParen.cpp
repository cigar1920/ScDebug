#include "InsertParen.h"

int main(int argc, const char **argv) {
	CommonOptionsParser op(argc, argv, MyToolCategory);        
	ClangTool Tool(op.getCompilations(), op.getSourcePathList());
	int result = Tool.run(newFrontendActionFactory<InsertParenFrontendAction>().get());
	if (isChanged) {
		freopen((op.getSourcePathList().front() + ip).c_str(), "w", stdout);
		rewriter.getEditBuffer(rewriter.getSourceMgr().getMainFileID()).write(outs());
	}
	return 0;
}