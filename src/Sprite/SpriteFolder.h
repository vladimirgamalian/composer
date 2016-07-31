#pragma once
#include "TreeModel/TreeNode.h"

class SpriteFolder : public TreeNode
{
public:
	SpriteFolder( QString name )
	{
		text = name;
		icon = QIcon( ":/composer/Resources/icons/folderClose" );
	}

	virtual SpriteFolder* clone() override
	{
		SpriteFolder* newNode = new SpriteFolder( text );
		cloneChild( newNode );
		return newNode;
	}

	virtual NodeType getType() const
	{
		return NodeType::Folder;
	}

private:

};
