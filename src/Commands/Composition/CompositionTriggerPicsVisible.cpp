#include "stdafx.h"
#include "CompositionTriggerPicsVisible.h"

CompositionTriggerPicsVisible::CompositionTriggerPicsVisible(CommandEnv* commandEnv,
	QString spritePath, int frameIndex, const QList<int>& pics)
	: BaseCommand(commandEnv),
	spritePath(spritePath), frameIndex(frameIndex), pics(pics)
{
	setText("toggle visibility");
}

void CompositionTriggerPicsVisible::redo()
{
	commandEnv->restore();
	commandEnv->project->compositionPicturesToggleVisible(spritePath, frameIndex, pics);
}

void CompositionTriggerPicsVisible::undo()
{
	commandEnv->project->compositionPicturesToggleVisible(spritePath, frameIndex, pics);
	commandEnv->restore();
}
