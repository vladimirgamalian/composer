#include "stdafx.h"
#include "AnimationReverseCommand.h"

AnimationReverseCommand::AnimationReverseCommand(CommandEnv* commandEnv, 
	QString spritePath, const QList<int>& indexes)
	: AnimationBaseCommand(commandEnv, spritePath), indexes(indexes)
{
	setText(QString("reverse frames ") + spritePath);
}

void AnimationReverseCommand::action()
{
	commandEnv->project->animReverse(spritePath, indexes);
}
