#pragma once
#include "CompositionBaseCommand.h"

class CompositionMovePictures : public CompositionBaseCommand
{
public:
	CompositionMovePictures(CommandEnv* commandEnv, QString spritePath, int frameIndex,
		const QList<Project::MovePicData>& moveData);

protected:
	virtual void action();

private:
	QList<Project::MovePicData> moveData;
};
