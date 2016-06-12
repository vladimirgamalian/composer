#pragma once
#include "CompositionBaseCommand.h"

class CompositionDeletePicturesCommand : public CompositionBaseCommand
{
public:
	CompositionDeletePicturesCommand(CommandEnv* commandEnv, QString spritePath, int frameIndex,
		const QList<int>& frames);

protected:
	virtual void action();

private:
	QList<int> frames;
};
