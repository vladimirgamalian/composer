#include "stdafx.h"
#include "AnimationChangeTagCommand.h"

AnimationChangeTagCommand::AnimationChangeTagCommand(CommandEnv* commandEnv,
	QString spritePath, const QList<int>& frames, QString value)
	: AnimationBaseCommand(commandEnv, spritePath), frames(frames), value(value)
{
	setText("change frame tag");
}

void AnimationChangeTagCommand::action()
{
	Q_ASSERT(!frames.isEmpty());
	commandEnv->project->animSetTag(spritePath, frames, value);
}
