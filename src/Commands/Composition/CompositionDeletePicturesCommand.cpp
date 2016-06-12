#include "stdafx.h"
#include "CompositionDeletePicturesCommand.h"

CompositionDeletePicturesCommand::CompositionDeletePicturesCommand(CommandEnv* commandEnv, QString spritePath,
	int frameIndex, const QList<int>& frames) : CompositionBaseCommand(commandEnv, spritePath, frameIndex),
	frames(frames)
{
	setText(QString("delete pictures"));
}

void CompositionDeletePicturesCommand::action()
{
	commandEnv->project->compositionDelItems(spritePath, frameIndex, frames);
}
