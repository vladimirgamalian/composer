#pragma once
#include "TreeModel/TreeNode.h"

class SpriteRoot : public TreeNode
{
public:
	SpriteRoot()
	{
		text = "resources";
		icon = QIcon(":/composer/Resources/icons/folderRoot");
	}

	virtual TreeNode* clone()
	{
		Q_ASSERT(false);
		SpriteRoot* newNode = new SpriteRoot();
		cloneChild(newNode);
		return newNode;
	}

	virtual NodeType getType() const
	{
		return NodeType::Root;
	}

private:

};
