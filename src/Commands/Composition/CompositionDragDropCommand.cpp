#include "stdafx.h"
#include "CompositionDragDropCommand.h"

CompositionDragDropCommand::CompositionDragDropCommand(CommandEnv* commandEnv, QString spritePath, int frameIndex,
	const QList<int>& indexes, int row, bool copyAction) 
	: CompositionBaseCommand(commandEnv, spritePath, frameIndex),
	indexes(indexes), row(row), copyAction(copyAction)
{
	setText(QString("drag drop pictures"));
}

void CompositionDragDropCommand::action()
{
	commandEnv->project->compositionDragDrop(spritePath, frameIndex, indexes, row, copyAction);
}
