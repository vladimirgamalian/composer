#include "stdafx.h"
#include "TestTreeNode.h"
#include "TestIntervalSolver.h"
#include "TestScopedBool.h"

int main(int argc, char *argv[])
{
	TestTreeNode testTreeNode;
	QTest::qExec(&testTreeNode);

	TestIntervalSolver testIntervalSolver;
	QTest::qExec(&testIntervalSolver);

	TestScopedBool testScopedBool;
	QTest::qExec(&testScopedBool);
	
	return 0;
}
