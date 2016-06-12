#include "stdafx.h"
#include "TestTreeNode.h"
#include "../../src/TreeModel/TreeNode.h"
#include "../../src/Sprite/SpriteFolder.h"

void TestTreeNode::splitChild()
{
	SpriteFolder rootNode("test");
	SpriteFolder* childNode0 = new SpriteFolder("test");
	SpriteFolder* childNode1 = new SpriteFolder("test");
	SpriteFolder* childNode2 = new SpriteFolder("test");
	SpriteFolder* childNode3 = new SpriteFolder("test");

	rootNode.appendChild(childNode0);
	rootNode.appendChild(childNode1);
	childNode1->appendChild(childNode2);
	childNode2->appendChild(childNode3);

	int row;
	TreeNode* node = rootNode.splitChild("/1/0/0/3", &row);
	QVERIFY(node != nullptr);
	QCOMPARE(node->toString(), QString("/1/0/0"));
	QCOMPARE(row, 3);

	node = rootNode.splitChild("/1/0", &row);
	QVERIFY(node != nullptr);
	QCOMPARE(node->toString(), QString("/1"));
	QCOMPARE(row, 0);

	node = rootNode.splitChild("/1", &row);
	QVERIFY(node != nullptr);
	QCOMPARE(node->toString(), QString("/"));
	QCOMPARE(row, 1);

	node = rootNode.splitChild("", &row);
	QVERIFY(node == nullptr);
	node = rootNode.splitChild("/1/0/2/3", &row);
	QVERIFY(node == nullptr);
}

void TestTreeNode::tree()
{
	SpriteFolder rootNode("test");
	SpriteFolder* childNode0 = new SpriteFolder("test");
	SpriteFolder* childNode1 = new SpriteFolder("test");
	SpriteFolder* childNode2 = new SpriteFolder("test");
	SpriteFolder* childNode3 = new SpriteFolder("test");

	rootNode.appendChild(childNode0);
	rootNode.appendChild(childNode1);
	childNode1->appendChild(childNode2);
	childNode2->appendChild(childNode3);

	QCOMPARE(rootNode.toString(), QString("/"));
	QCOMPARE(childNode0->toString(), QString("/0"));
	QCOMPARE(childNode1->toString(), QString("/1"));
	QCOMPARE(childNode2->toString(), QString("/1/0"));
	QCOMPARE(childNode3->toString(), QString("/1/0/0"));
}

void TestTreeNode::emptyConstructor()
{
	SpriteFolder spriteTreePath("test");

	QVERIFY(spriteTreePath.isRoot());
	QCOMPARE(spriteTreePath.toString(), QString("/"));
}

// void TestTreeNode::fromMalfunctionString()
// {
// 	TreeNode spriteTreePath;
// 	QVERIFY(!spriteTreePath.fromString(""));
// 	QVERIFY(!spriteTreePath.fromString("1"));
// 	QVERIFY(!spriteTreePath.fromString("/1/2/"));
// 	QVERIFY(!spriteTreePath.fromString("1/2/"));
// 	QVERIFY(!spriteTreePath.fromString("/1a/2"));
// 	QVERIFY(!spriteTreePath.fromString("/1/-1"));
// 	QVERIFY(!spriteTreePath.fromString("/01234567891"));
// }

// void TestTreeNode::fromString()
// {
// 	TreeNode spriteTreePath;
// 	QVERIFY(spriteTreePath.fromString("/"));
// 	QVERIFY(spriteTreePath.isRoot());
// 	QCOMPARE(spriteTreePath.toString(), QString("/"));
// 
// 	QVERIFY(spriteTreePath.fromString("/1"));
// 	QVERIFY(!spriteTreePath.isRoot());
// 	QCOMPARE(spriteTreePath.toString(), QString("/1"));
// 
// 	QVERIFY(spriteTreePath.fromString("/1/2/3"));
// 	QVERIFY(!spriteTreePath.isRoot());
// 	QCOMPARE(spriteTreePath.toString(), QString("/1/2/3"));
// }

// void TestTreeNode::append()
// {
// 	TreeNode spriteTreePath;
// 	QVERIFY(spriteTreePath.fromString("/"));
// 	QVERIFY(spriteTreePath.isRoot());
// 	QCOMPARE(spriteTreePath.toString(), QString("/"));
// 
// 	spriteTreePath.append(0);
// 	QVERIFY(!spriteTreePath.isRoot());
// 	QCOMPARE(spriteTreePath.toString(), QString("/0"));
// 
// 	spriteTreePath.append(1);
// 	QVERIFY(!spriteTreePath.isRoot());
// 	QCOMPARE(spriteTreePath.toString(), QString("/0/1"));
// }
// 
// QTEST_MAIN(TestTreeNode)
// #include "TestTreeNode.moc"
