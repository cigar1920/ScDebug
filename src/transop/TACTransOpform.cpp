
#include "TACTransOpform.h"

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
