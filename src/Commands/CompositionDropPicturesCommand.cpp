#include "stdafx.h"
#include "CompositionDropPicturesCommand.h"

CompositionDropPicturesCommand::CompositionDropPicturesCommand(CommandEnv* commandEnv, QString spritePath, int frameIndex,
	int compositionIndex, QPoint pos, QStringList fileList )
	: CompositionBaseCommand(commandEnv, spritePath, frameIndex),
	compositionIndex(compositionIndex),	pos(pos), fileList(fileList)
{
	setText(QString("drop pictures"));
}

void CompositionDropPicturesCommand::action()
{
	commandEnv->project->compositionDropPictures(spritePath, frameIndex, compositionIndex, pos, fileList);
}
