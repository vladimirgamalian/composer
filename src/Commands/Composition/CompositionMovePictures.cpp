#include "stdafx.h"
#include "CompositionMovePictures.h"

CompositionMovePictures::CompositionMovePictures(CommandEnv* commandEnv, QString spritePath, int frameIndex,
	const QList<Project::MovePicData>& moveData) : CompositionBaseCommand(commandEnv, spritePath, frameIndex),
	moveData(moveData)
{
	setText(QString("move pictures"));
}

void CompositionMovePictures::action()
{
	commandEnv->project->scenePicturesMove(spritePath, frameIndex, moveData);
}
