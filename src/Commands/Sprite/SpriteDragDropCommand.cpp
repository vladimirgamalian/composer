#include "stdafx.h"
#include "SpriteDragDropCommand.h"

SpriteDragDropCommand::SpriteDragDropCommand(CommandEnv* commandEnv,
	QString oldPath, QString newPath, bool copy) : 
	BaseCommand(commandEnv),
	oldPath(oldPath), newPath(newPath), row(row), copy(copy)
{
	setText(QString(copy ? "Copy " : "Move") + QString(" sprite node from  ") + oldPath + QString(" to ") + newPath);
}

void SpriteDragDropCommand::redo()
{
	qDebug() << "(redo) " << (copy ? "copy " : "move") 
		<< " sprite node from " << oldPath << "to" << newPath;

	commandEnv->restore();
	commandEnv->project->spriteDragDrop(oldPath, newPath, copy);

	//TODO: always select newPath
	commandEnv->spriteView->setCurrentNode(copy ? oldPath : newPath);
}

void SpriteDragDropCommand::undo()
{
	if (copy)
	{
		qDebug() << "(undo) remove sprite node from " << newPath;
		delete commandEnv->project->spriteUnlinkNode(newPath);
	}
	else
	{
		qDebug() << "(undo) move sprite node from " << newPath << "to" << oldPath;
		commandEnv->project->spriteDragDrop(newPath, oldPath, false);
	}

	commandEnv->restore();
}
