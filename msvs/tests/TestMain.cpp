#include "stdafx.h"
#include "TestTreeNode.h"
#include "TestIntervalSolver.h"

int main(int argc, char *argv[])
{
	TestTreeNode testTreeNode;
	QTest::qExec(&testTreeNode);

	TestIntervalSolver testIntervalSolver;
	QTest::qExec(&testIntervalSolver);
	
	return 0;
}
