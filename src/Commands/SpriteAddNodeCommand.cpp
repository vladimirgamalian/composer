#include "stdafx.h"
#include "SpriteAddNodeCommand.h"

SpriteAddNodeCommand::SpriteAddNodeCommand(CommandEnv* commandEnv,
	QString parentPath, TreeNode::NodeType nodeType) :
	BaseCommand(commandEnv),
	parentPath(parentPath), nodeType(nodeType)
{
	setText(QString("add node to ") + parentPath);
}

void SpriteAddNodeCommand::redo()
{
	commandEnv->restore();
	nodePath = commandEnv->project->spriteAddNode(parentPath, "sprite", nodeType);
	commandEnv->spriteView->setCurrentNode(nodePath);
}

void SpriteAddNodeCommand::undo()
{
	delete commandEnv->project->spriteUnlinkNode(nodePath);
	commandEnv->restore();
}
