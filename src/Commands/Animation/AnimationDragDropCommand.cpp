#include "stdafx.h"
#include "AnimationDragDropCommand.h"

AnimationDragDropCommand::AnimationDragDropCommand(CommandEnv* commandEnv, QString spritePath,
	const QList<int>& indexes, int row, bool copyAction) : AnimationBaseCommand(commandEnv, spritePath),
	indexes(indexes), row(row), copyAction(copyAction)

{
	setText(QString("drag drop frames"));
}

void AnimationDragDropCommand::action()
{
	commandEnv->project->animDragDrop(spritePath, indexes, row, copyAction);
}
