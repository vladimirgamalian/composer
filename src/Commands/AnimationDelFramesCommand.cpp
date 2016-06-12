#include "stdafx.h"
#include "AnimationDelFramesCommand.h"

AnimationDelFramesCommand::AnimationDelFramesCommand(CommandEnv* commandEnv, QString spritePath, const QList<int>& frames)
	: AnimationBaseCommand(commandEnv, spritePath),
	frames(frames)
{
	setText(QString("delete frames"));
}

void AnimationDelFramesCommand::action()
{
	commandEnv->project->animRemoveFrames(spritePath, frames);
}
