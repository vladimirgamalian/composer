#include "stdafx.h"
#include "SpritesDeleteNodeCommand.h"

SpritesDeleteNodeCommand::SpritesDeleteNodeCommand(CommandEnv* commandEnv, QString nodePath) :
	BaseCommand(commandEnv), nodePath(nodePath)
{
	setText(QString("delete node ") + nodePath);
}

SpritesDeleteNodeCommand::~SpritesDeleteNodeCommand()
{
	delete deletedNode;
}

void SpritesDeleteNodeCommand::redo()
{
	commandEnv->restore();
	deletedNode = commandEnv->project->spriteUnlinkNode(nodePath);
}

void SpritesDeleteNodeCommand::undo()
{
	commandEnv->project->spriteLinkNode(nodePath, deletedNode);
	deletedNode = nullptr;
	commandEnv->restore();
}
