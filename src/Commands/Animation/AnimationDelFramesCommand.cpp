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

	int newFrameCount = commandEnv->project->animGetRowCount(spritePath);

	if ((!newFrameCount) || (frames.size() != 1))
		commandEnv->animationView->setSelected(QList<int>());
	else
	{
		int frame = frames[0];
		if (frame >= newFrameCount)
			frame--;
		commandEnv->animationView->setSelected(QList<int>{frame});
	}
}
