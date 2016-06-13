#pragma once
#include "Commands/BaseCommand.h"

class CompositionMovePictures : public BaseCommand
{
public:
	CompositionMovePictures(CommandEnv* commandEnv, QString spritePath, int frameIndex,
		const QList<Project::MovePicData>& moveData);

	void redo() override;
	void undo() override;

private:
	QString spritePath;
	int frameIndex;
	QList<Project::MovePicData> moveData;
};
