#include "stdafx.h"
#include "AnimationInsertFrameCommand.h"

AnimationInsertFrameCommand::AnimationInsertFrameCommand(CommandEnv* commandEnv, QString spritePath, int index,
	bool before, bool copyContent)
	: AnimationBaseCommand(commandEnv, spritePath),
	index(index), before(before), copyContent(copyContent)
{
	setText(QString("insert frame ") + QString::number(index));
}

void AnimationInsertFrameCommand::action()
{
	newIndex = commandEnv->project->animInsertFrame(spritePath, index, before, copyContent);
}
