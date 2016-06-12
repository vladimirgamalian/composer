#include "stdafx.h"
#include "TestIntervalSolver.h"
#include "../../src/IntervalSorter.h"

void TestIntervalSolver::emptyList()
{
	QList<int> src;
	QList<IntervalSorter::Item> res = IntervalSorter::sort(src);
	QVERIFY(res.empty());
}

void TestIntervalSolver::basicTest0()
{
	QList<int> src({ 2 });
	QList<IntervalSorter::Item> expectedResult({ { 2,2 } });
	QList<IntervalSorter::Item> res = IntervalSorter::sort(src);
	QCOMPARE(res, expectedResult);
}

void TestIntervalSolver::basicTest1()
{
	QList<int> src({ 2, 3});
	QList<IntervalSorter::Item> expectedResult({ { 2,3 } });
	QList<IntervalSorter::Item> res = IntervalSorter::sort(src);
	QCOMPARE(res, expectedResult);
}

void TestIntervalSolver::basicTest2()
{
	QList<int> src({ 2, 4 });
	QList<IntervalSorter::Item> expectedResult({ { 2,2 },{ 4,4 } });
	QList<IntervalSorter::Item> res = IntervalSorter::sort(src);
	QCOMPARE(res, expectedResult);
}

void TestIntervalSolver::basicTest3()
{
	QList<int> src({ 2, 3, 5, 6, 7, 8, 10, 12, 15, 16, 20 });
	QList<IntervalSorter::Item> expectedResult({ { 2,3 },{ 5,8 },{ 10,10 },{ 12,12 },{ 15,16 },{ 20,20 } });
	QList<IntervalSorter::Item> res = IntervalSorter::sort(src);
	QCOMPARE(res, expectedResult);
}
