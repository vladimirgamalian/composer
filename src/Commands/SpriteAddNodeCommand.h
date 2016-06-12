#pragma once
#include "Project.h"
#include "BaseCommand.h"

class SpriteAddNodeCommand : public BaseCommand
{
public:
	SpriteAddNodeCommand(CommandEnv* commandEnv,
		QString parentPath, TreeNode::NodeType nodeType);
	void redo() override;
	void undo() override;

private:
	QString parentPath;
	TreeNode::NodeType nodeType;
	QString nodePath;
};
