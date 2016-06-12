#pragma once
#include "CompositionBaseCommand.h"

class CompositionDropPicturesCommand : public CompositionBaseCommand
{
public:
	CompositionDropPicturesCommand(CommandEnv* commandEnv, QString spritePath, int frameIndex,
		int compositionIndex, QPoint pos, QStringList fileList);

protected:
	virtual void action();

private:
	int compositionIndex;
	QPoint pos;
	QStringList fileList;
};
