#include "stdafx.h"
#include "SpriteRenameCommand.h"

SpriteRenameCommand::SpriteRenameCommand(CommandEnv* commandEnv, SpriteModel* spriteModel,
	QString nodePath, QString newName)  : BaseCommand(commandEnv),
	spriteModel(spriteModel), nodePath(nodePath), newName(newName)
{
	setText(QString("rename node ") + nodePath + QString(" to ") + newName);
}

void SpriteRenameCommand::redo()
{
	commandEnv->restore();
	oldName = commandEnv->project->spriteNodeRename(nodePath, newName);
	spriteModel->nodeDataChanged(nodePath);
}

void SpriteRenameCommand::undo()
{
	commandEnv->project->spriteNodeRename(nodePath, oldName);
	spriteModel->nodeDataChanged(nodePath);
	commandEnv->restore();
}
