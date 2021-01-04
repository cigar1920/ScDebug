
#include "PartionStmt.h"
int main(int argc, const char **argv)
{
	CommonOptionsParser op(argc, argv, MyToolCategory);
	ClangTool Tool(op.getCompilations(), op.getSourcePathList());
	int result = Tool.run(newFrontendActionFactory<PartionStmtFrontendAction>().get());
	// sort(underBlock.begin(),underBlock.end(),cmp);
	set<int>::iterator it = underBlock.begin();
	ofstream partionFile("./log/partion.log", ios::out | ios::app);
	partionFile << "1 ";
	for (; it != underBlock.end(); it++)
	{

		partionFile << *it;
		partionFile << endl;
		partionFile << *it<<" ";
	}
	partionFile << -1;
	partionFile.close();
	return 0;
}
