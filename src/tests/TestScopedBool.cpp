#include "stdafx.h"
#include "TestScopedBool.h"
#include "../../src/Utils/ScopedBool.h"

void TestScopedBool::basic()
{
	bool foo = false;
	QVERIFY(!foo);
	{
		ScopedBool scopedBool(foo);
		QVERIFY(foo);
	}
	QVERIFY(!foo);
}
