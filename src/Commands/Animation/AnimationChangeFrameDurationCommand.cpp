#include "stdafx.h"
#include "AnimationChangeFrameDurationCommand.h"

AnimationChangeFrameDurationCommand::AnimationChangeFrameDurationCommand(CommandEnv* commandEnv, 
	QString spritePath, const QList<int>& frames, int value)
	: AnimationBaseCommand(commandEnv, spritePath), frames(frames), value(value)
{
	setText("change frame duration");
}

void AnimationChangeFrameDurationCommand::action()
{
	Q_ASSERT(!frames.isEmpty());
	commandEnv->project->animSetDuration(spritePath, frames, value);
}
