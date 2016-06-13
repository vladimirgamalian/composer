#include "stdafx.h"
#include "CompositionMovePictures.h"

CompositionMovePictures::CompositionMovePictures(CommandEnv* commandEnv, QString spritePath, int frameIndex,
	const QList<Project::MovePicData>& moveData) 
	: BaseCommand(commandEnv),
	spritePath(spritePath), frameIndex(frameIndex),	moveData(moveData)
{
	setText(QString("move pictures"));
}

void CompositionMovePictures::redo()
{
	commandEnv->restore();
	commandEnv->project->scenePicturesMove(spritePath, frameIndex, moveData, true);
}

void CompositionMovePictures::undo()
{
	commandEnv->project->scenePicturesMove(spritePath, frameIndex, moveData, false);
	commandEnv->restore();
}
